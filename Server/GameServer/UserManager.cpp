#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void GSUserManager::ProcessSave()
{
	GSAccount* Account = GSAccountManager::GetInstance()->GetAccount(7);  // AccountLock

	lock_guard<mutex> LockGuard(Mutex);                                   // UserLock
}
