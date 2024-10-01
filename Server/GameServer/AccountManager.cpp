#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void GSAccountManager::ProcessLogin()
{
	lock_guard<mutex> LockGuard(Mutex);                       // AccountLock

	GSUser* User = GSUserManager::GetInstance()->GetUser(100);  // UserLock
}
