#pragma once

#include <mutex>

template<typename T>
class LockStack 
{
private:
	stack<T> m_sStack;
	mutex m_mMutex;
	condition_variable m_cvCondVar;

public:
	LockStack() {};

	LockStack(const LockStack&) = delete;
	LockStack& operator = (const LockStack&) = delete;

public:
	void Push(T _value) {
		lock_guard<mutex> lock(m_mMutex);
		m_sStack.push(move(_value));
		m_cvCondVar.notify_one();
	}

	bool TryPop(T& _value) {
		lock_guard<mutex> lock(m_mMutex);
		if(m_sStack.empty())
			return false;

		_value = std::move(m_sStack.top());
		m_sStack.pop();
		return true;
	}

	bool Empty() {
		lock_guard<mutex> lock(m_mMutex);
		return m_sStack.empty();
	}

	void WaitPop(T& _value) {
		unique_lock<mutex> lock(m_mMutex);
		m_cvCondVar.wait(lock, [this]() {return !m_sStack.empty(); });
		_value = std::move(m_sStack.pop());
		m_sStack.pop();
	}
};

template<typename T>
class LockFreeStack
{
	struct Node {
		Node(const T& value) : data(value), next(nullptr)
		{

		}

		T data;
		Node* next;
	};

private:
	atomic<Node*> m_aHead;
	atomic<uint32> m_aPopCount = 0; // Pop�� �������� �������� ����
	atomic<Node*> m_aPendingList; //���� �Ǿ�� �� ����(ù��°�� ���)

public:
	// 1) �� ��带 �����
	// 2) �� ����� next = head
	// 3) head = �� ���
	void Push(const T& _value) {
		Node* node = new Node(_value);
		node->next = m_aHead;//��尡 ������ �ڿ����� nullptr�� ��.

		//�� ���̿� ��ġ�� ���ϸ�?
		//_head = node;

		while (m_aHead.compare_exchange_weak(node->next, node) == false) {
			//���Ѵ��
		}
	}

	//1) head �б�
	// 2) head->next �б�
	// 3) head = head->next
	// 4) data�����ؼ� ��ȯ
	// 5) ������ ��带 ����
	bool TryPop(T& _value) {
		++m_aPopCount;

		Node* oldHead = m_aHead;

		while (oldHead && m_aHead.compare_exchange_weak(oldHead, oldHead->next) == false) {

		}

		if (oldHead == nullptr) {
			--m_aPopCount;
			return false;
		}

		//Exception X
		_value = oldHead->data;
		TryDelete(oldHead);

		//���� ����
		//delete oldHead;
		return true;
	}

	// 1) ������ �и�
	// 2) Count üũ
	// 3) �� ȥ�ڸ� ����
	void TryDelete(Node* oldHead) {
		//�� �ܿ� ���� �õ��ϴ� �����尡 �ִ°�?
		if (m_aPopCount==1) {
			//�� ȥ�ڳ�?
			//�����͸� �и� �߱� ������ (head�� �ڷ� ����, ���� ���ڸ� ���Ҵ� �� �����尡 �̹� ������ ����)
			//�׷��� �ٷ� ���� ����

			//���� �����ʹ� ����
			delete oldHead;

			//�̿� ȥ���ΰ� ���� ����� �ٸ� �����͵鵵(pending list) �����ϱ�
			Node* node = m_aPendingList.exchange(nullptr);
			//�ٵ� pending list�� �̹� ���� �ǵ帮�� ���� �� �־ ���� ������ �� �ִ��� üũ�������
			// 1) ������ �и�, 2) Count üũ, 3) �� ȥ�ڸ� ���� 3���� ������ �� ��Ű�鼭 �õ��غ���.			

			if (--m_aPopCount == 0) {//������ �� �����忡 �� ȥ�ڸ� �ִٸ�
				//�߰��� ����� �ְ� ���ٸ� ���� ����
				// �����ͼ� �������� ����� ������ �����ʹ� �и��ص� ����!
				DeleteNodes(node);
			}
			else if(node) {
				//���� ���������� �ٽ� ������ ����
				ChainPendingNodeList(node);
			}
		}
		else {
			//���� �ֳ�? �׷� ���� �������� �ʰ� pending list�� �߰�����
			--m_aPopCount;
			ChainPendingNode(oldHead);
		}
	}

	//������ ����Ʈ(pending list)�� ���� ��������Ʈ�� �ڿ� ��������ش�.
	//pending list�� ���� ���ϸ� �ٽ� �̾�پ�����ϱ� ������ �� �Լ��� ����
	void ChainPendingNodeList(Node* first, Node* last) {
		last->next = m_aPendingList;

		while (m_aPendingList.compare_exchange_weak(last->next, first) == false) {

		}
	}
	
	//�̰� ù���� ��� ������ �˾Ƽ� ���ڸ� ã�Ƽ� ��������.
	void ChainPendingNodeList(Node* node) {
		Node* last = node;
		while (last->next)
			last = last->next;

		ChainPendingNodeList(node, last);
	}

	//�� �ϳ�¥���� �������� ��(Ư���� ���)
	void ChainPendingNode(Node* node) {
		ChainPendingNodeList(node);
	}

	static void DeleteNodes(Node* node) {
		while (node) {
			Node* next = node->next;
			delete node;
			node = next;
		}
	}
};

//template<typename T>
//class LockFreeStackV2
//{
//	struct Node {
//		Node(const T& value) : data(make_shared<T>(value)), next(nullptr)
//		{
//
//		}
//
//		shared_ptr<T> data;
//		shared_ptr<Node> next;
//	};
//
//private:
//	shared_ptr<Node> m_aHead;
//
//public:
//	void Push(const T& _value) {
//		shared_ptr<Node> node = make_shared<Node>(_value);
//		node->next = atomic_load(&m_aHead);		
//
//		while (atomic_compare_exchange_weak(&m_aHead, &node->next, node) == false) {
//		}
//	}
//
//	shared_ptr<T> TryPop() {
//		shared_ptr<Node> oldHead = atomic_load(&m_aHead);
//
//		while (oldHead && atomic_compare_exchange_weak(&m_aHead, &oldHead, oldHead->next) == false) {
//
//		}
//
//		if (oldHead == nullptr)
//			return shared_ptr<T>();
//
//		return oldHead->data;
//	}
//
//};

template<typename T>
class LockFreeStackV2
{
	struct Node;

	struct CountedNodePtr {
		int32 externalCount = 0;
		Node* ptr = nullptr;
	};

	struct Node {
		Node(const T& value) : data(make_shared<T>(value))
		{

		}

		shared_ptr<T> data;
		atomic<int32> internalCount = 0;
		CountedNodePtr next;
	};

private:
	atomic<CountedNodePtr> m_aHead;

public:
	void Push(const T& _value) {
		CountedNodePtr node;
		node.ptr = new Node(_value);
		node.externalCount = 1;
		//������� �ƹ� ��� ����
		node.ptr->next = m_aHead;
		while (m_aHead.compare_exchange_weak(node.ptr->next, node) == false) {

		}

	}

	shared_ptr<T> TryPop() {
		CountedNodePtr oldHead = m_aHead;
		while (true) {
			//Head �� ���� ������ �����;���.
			//Head�� ���� ���ڸ� �ϳ� �������Ѿ���.
			//������Ű�� �����常�� ���� ȹ�� ����
			//������ ȹ��(externalCount�� �� �������� +1 �� �ְ� �̱�)
			IncreaseHeadCount(oldHead);
			//�ּ��� externalCount >=2���״�  ����x(�����ϰ� ������ �� �ִ�)
			Node* ptr = oldHead.ptr;

			//������ ����
			if (ptr == nullptr)
				return shared_ptr<T>();

			//���� ������ �����ϱ� ���� �������� ȹ���ؾ���.
			//������ ȹ��(ptr->next�� head�� �ٲ�ġ�� �� �ְ� �̱�)
			//�����Ǳ��� ���� Head�� ��������.
			if (m_aHead.compare_exchange_strong(oldHead, ptr->next)) {
				shared_ptr<T> res;
				res.swap(ptr->data);

				//�� ���� �� ���� �ִ°�?
				const int32 countIncrease = oldHead.externalCount - 2;
				if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease) 
					delete ptr;				

				return res;
			}
			else if(ptr->internalCount.fetch_sub(1)==1) {
				//�������� ������� �������� ����-> �޼����� ���� �Ѵ�.
				delete ptr;
			}
		}
	}

private:
	void IncreaseHeadCount(CountedNodePtr& oldCounter) {
		while (true) {
			CountedNodePtr newCounter = oldCounter;
			newCounter.externalCount++;

			if (m_aHead.compare_exchange_strong(oldCounter, newCounter)) {
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}
};