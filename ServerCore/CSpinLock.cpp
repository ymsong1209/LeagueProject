#include "pch.h"
#include "CSpinLock.h"

void CSpinLock::lock()
{
	//CAS(Compare-And-Swap)

	bool expected = false;
	bool desired = true;

	//CAS 의사 코드
	//if (_locked == expected) {
	//	expected = _locked;
	//	_locked = desired;
	//	return true;
	//}
	//else {
	//	expected = _locked;
	//	return false;
	//}

	while (_locked.compare_exchange_strong(expected, desired) == false) {
		expected = false;
	}
}

void CSpinLock::unlock()
{ 
	_locked.store(false);
}
