#ifndef __FASTMUTEX_H__
#define __FASTMUTEX_H__

#include <fltKernel.h>

class FastMutex {
public:
	void Init();
	void Lock();
	void Unlock();
private:
	FAST_MUTEX _mtx;
};

#endif // __FASTMUTEX_H__
#pragma once
