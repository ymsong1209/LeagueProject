#pragma once
class CSleep
{
private:
	atomic<bool> _locked = false;

public:
	void lock();
	void unlock();
};

