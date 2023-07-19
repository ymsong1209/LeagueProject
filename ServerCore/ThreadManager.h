#pragma once

#include <thread>
#include <functional>

/*------------------
	ThreadManager
-------------------*/

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void	Launch(function<void(void)> callback);
	void	Join();

	void SetFlags(int flags) { _flags.store(flags); }
	int GetFlags() { return _flags; }

	static void InitTLS();
	static void DestroyTLS();

private:
	Mutex			_lock;
	vector<thread>	_threads;
	atomic<int> _flags;
};

