#pragma once

#include<mutex>

template <typename T>
class LockQueue {
private:
	queue<T> m_qQueue;
	mutex m_mMutex;
	condition_variable m_cvCondVar;

public:
	LockQueue() {}

	LockQueue(const LockQueue&) = delete;
	LockQueue& operator = (const LockQueue&) = delete;


public:
	void Push(T _value) {
		lock_guard<mutex> lock(m_mMutex);
		m_qQueue.push(move(_value));
		m_cvCondVar.notify_one();
	}

	bool TryPop(T& _value) {
		lock_guard<mutex> lock(m_mMutex);
		
		if (m_qQueue.empty())
			return false;

		_value = move(m_qQueue.front());
		m_qQueue.pop();
		return true;
	}

	bool Empty() {
		lock_guard<mutex> lock(m_mMutex);

		return m_qQueue.empty();
	}

	void WaitPop(T& _value) {
		unique_lock<mutex> lock(m_mMutex);
		m_cvCondVar.wait(lock, [this]() {return !m_qQueue.empty(); });
		_value = move(m_qQueue.front());
		m_qQueue.pop();
	}
};

template<typename T>
class LockFreeQueue
{
	struct Node;

	struct CountedNodePtr
	{
		int32 externalCount; // ������
		Node* ptr = nullptr;
	};

	struct NodeCounter
	{
		uint32 internalCount : 30; // ������ ��ȯ ����
		uint32 externalCountRemaining : 2; // Push & Pop ���� ������ ����
	};

	struct Node
	{
		Node()
		{
			NodeCounter newCount;
			newCount.internalCount = 0;
			newCount.externalCountRemaining = 2;
			count.store(newCount);

			next.ptr = nullptr;
			next.externalCount = 0;
		}

		void ReleaseRef()
		{
			NodeCounter oldCounter = count.load();

			while (true)
			{
				NodeCounter newCounter = oldCounter;
				newCounter.internalCount--;

				// ����� �� ����
				if (count.compare_exchange_strong(oldCounter, newCounter))
				{
					if (newCounter.internalCount == 0 && newCounter.externalCountRemaining == 0)
						delete this;

					break;
				}
			}
		}

		atomic<T*> data;
		atomic<NodeCounter> count;
		CountedNodePtr next;
	};

public:
	LockFreeQueue()
	{
		CountedNodePtr node;
		node.ptr = new Node;
		node.externalCount = 1;

		_head.store(node);
		_tail.store(node);
	}

	LockFreeQueue(const LockFreeQueue&) = delete;
	LockFreeQueue& operator=(const LockFreeQueue&) = delete;

	// [data][]
	// [head][tail]
	void Push(const T& value)
	{
		unique_ptr<T> newData = make_unique<T>(value);

		CountedNodePtr dummy;
		dummy.ptr = new Node;
		dummy.externalCount = 1;

		CountedNodePtr oldTail = _tail.load(); // ptr = nullptr

		while (true)
		{
			// ������ ȹ�� (externalCount�� ������ ���� +1 �� �ְ� �̱�)
			IncreaseExternalCount(_tail, oldTail);

			// ������ ȹ�� (data�� ������ ��ȯ�� �ְ� �̱�)
			T* oldData = nullptr;
			if (oldTail.ptr->data.compare_exchange_strong(oldData, newData.get()))
			{
				oldTail.ptr->next = dummy;
				oldTail = _tail.exchange(dummy);
				FreeExternalCount(oldTail);
				newData.release(); // �����Ϳ� ���� unique_ptr�� ������ ����
				break;
			}

			// ������ ���� �й�..
			oldTail.ptr->ReleaseRef();
		}
	}

	shared_ptr<T> TryPop()
	{
		// [data][data][ ]
		// [head][tail]

		CountedNodePtr oldHead = _head.load();

		while (true)
		{
			// ������ ȹ�� (externalCount�� ������ ���� +1 �� �ְ� �̱�)
			IncreaseExternalCount(_head, oldHead);

			Node* ptr = oldHead.ptr;
			if (ptr == _tail.load().ptr)
			{
				ptr->ReleaseRef();
				return shared_ptr<T>();
			}

			// ������ ȹ�� (head = ptr->next)
			if (_head.compare_exchange_strong(oldHead, ptr->next))
			{
				T* res = ptr->data.load(); // exchange(nullptr); �� �ϸ� ���� ����!
				FreeExternalCount(oldHead);
				return shared_ptr<T>(res);
			}

			ptr->ReleaseRef();
		}
	}

private:
	static void IncreaseExternalCount(atomic<CountedNodePtr>& counter, CountedNodePtr& oldCounter)
	{
		while (true)
		{
			CountedNodePtr newCounter = oldCounter;
			newCounter.externalCount++;

			if (counter.compare_exchange_strong(oldCounter, newCounter))
			{
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

	static void FreeExternalCount(CountedNodePtr& oldNodePtr)
	{
		Node* ptr = oldNodePtr.ptr;
		const int32 countIncrease = oldNodePtr.externalCount - 2;

		NodeCounter oldCounter = ptr->count.load();

		while (true)
		{
			NodeCounter newCounter = oldCounter;
			newCounter.externalCountRemaining--; // TODO
			newCounter.internalCount += countIncrease;

			if (ptr->count.compare_exchange_strong(oldCounter, newCounter))
			{
				if (newCounter.internalCount == 0 && newCounter.externalCountRemaining == 0)
					delete ptr;

				break;
			}
		}
	}

private:
	// [data][data][]
	// [head][tail]
	atomic<CountedNodePtr> _head;
	atomic<CountedNodePtr> _tail;
};

