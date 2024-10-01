#pragma once

#include <mutex>

class GSAccount
{

};

class GSAccountManager
{
public:
	static GSAccountManager* GetInstance()
	{
		static GSAccountManager AccountManagerInstance;
		return &AccountManagerInstance;
	}

	GSAccount* GetAccount(int32 InID)
	{
		lock_guard<mutex> LockGuard(Mutex);
		return nullptr;
	}

	void ProcessLogin();

private:
	mutex Mutex;

};

