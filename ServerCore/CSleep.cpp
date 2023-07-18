#include "pch.h"
#include "CSleep.h"

#include <thread>

void CSleep::lock()
{		
	//CAS(Compare-And-Swap)
	bool expected = false;
	bool desired = true;

	while (_locked.compare_exchange_strong(expected, desired) == false) {
		expected = false;

		this_thread::sleep_for(0ms);
		//this_thread::yield();
	}
}

void CSleep::unlock()
{
	_locked.store(false);
}
