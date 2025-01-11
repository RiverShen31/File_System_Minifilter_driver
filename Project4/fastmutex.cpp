#include "fastmutex.h"

void FastMutex::Init() {
	//KeInitializeMutex(&_mtx, 0);
	ExInitializeFastMutex(&_mtx);
}

void FastMutex::Lock() {
	//KeWaitForSingleObject(&_mtx, Executive, KernelMode, FALSE, NULL);
	ExAcquireFastMutex(&_mtx);
}

void FastMutex::Unlock() {
	//KeReleaseMutex(&_mtx, FALSE);
	ExReleaseFastMutex(&_mtx);
}