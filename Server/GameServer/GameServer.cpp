#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void InvokeProcessSaveFunction()
{
    for (uint32 i = 0; i < 100'000; ++i)
    {
        GSUserManager::GetInstance()->ProcessSave();
    }
}

void InvokeProcessLoginFunction()
{
    for (uint32 i = 0; i < 100'000; ++i)
    {
        GSAccountManager::GetInstance()->ProcessLogin();
    }
}

int main()
{
    thread t1(InvokeProcessSaveFunction);
    thread t2(InvokeProcessLoginFunction);

    if (t1.joinable() == true)
    {
        t1.join();
    }

    if (t2.joinable() == true)
    {
        t2.join();
    }

    cout << "Jobs done." << endl;

    return 0;
}
