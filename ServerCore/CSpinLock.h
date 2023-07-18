#pragma once
class CSpinLock
{
private:
	atomic<bool> _locked = false;

public:
	void lock();
	void unlock();
};

