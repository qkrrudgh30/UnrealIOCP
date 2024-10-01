#pragma once

#include <mutex>

class GSUser
{

};

class GSUserManager
{
public:
	static GSUserManager* GetInstance()
	{
		static GSUserManager Instance;
		return &Instance;
	}

	GSUser* GetUser(int32 InID)
	{
		lock_guard<mutex> LockGuard(Mutex);
		return nullptr;
	}

	void ProcessSave();

private:
	mutex Mutex;

};
