#include "pch.h"

int main()
{
    SCSocketUtils::Init();

    SharedPtrSCListener Listener = std::make_shared<SCListener>();
    Listener->StartAccept(SCNetAddress(L"127.0.0.1", 8080));

    // 스레드의 개수는 core 개수 혹은 1.5배 정도가 좋음. 그보다 많아지면 context switching만 일어나기 때문.
    for (int32 i = 0; i < 5; i++)
    {
        SCThreadManager::GetInstance().Launch([=]()
            {
                while (true)
                {
                    GIOCPCore.Dispatch();
                }
            });
    }

    SCThreadManager::GetInstance().Join();

    SCSocketUtils::Clear();

    return 0;
}
