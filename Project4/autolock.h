#ifndef __AUTOLOCK_H__
#define __AUTOLOCK_H__

template<typename TLock>
struct AutoLock {
	AutoLock(TLock& lock) : _lock(lock) {
		_lock.Lock();
	}

	~AutoLock() {
		_lock.Unlock();
	}

private:
	TLock& _lock;
};
#endif // !__AUTOLOCK_H__
