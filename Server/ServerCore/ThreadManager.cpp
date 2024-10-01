#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"

SCThreadManager::SCThreadManager()
{
    InitTLS();
}

SCThreadManager::~SCThreadManager()
{
    Join();
}

SCThreadManager& SCThreadManager::GetInstance()
{
    static SCThreadManager ThreadManagerInstance;
    return ThreadManagerInstance;
}

void SCThreadManager::Launch(function<void(void)> InCallback)
{
    std::lock_guard<std::mutex> Guard(Lock);

    Threads.push_back(std::thread([this, InCallback]() -> void
        {
            InitTLS();

            InCallback();

            DestroyTLS();
        }
    ));
}

void SCThreadManager::InitTLS()
{
    static std::atomic<uint32> ThreadID = 1;
    LThreadID = ThreadID.fetch_add(1);
}

void SCThreadManager::DestroyTLS()
{
}

void SCThreadManager::Join()
{
    for (std::thread& t : Threads)
    {
        if (t.joinable() == true)
        {
            t.join();
        }
    }

    Threads.clear();
}
