#include "pch.h"

class SCGameSession : public SCSession
{

};

int main()
{
    SCSocketUtils::Init();

    SharedPtrSCServerService ServerService = std::make_shared<SCServerService>(
        SCNetAddress(L"127.0.0.1", 8080),
        std::make_shared<SCIOCPCore>(),
        [=]() -> SharedPtrSCSession { return std::make_shared<SCGameSession>(); },
        100
    );

    bool bStartSuccedded = ServerService->Start();
    assert(bStartSuccedded == true && "bStartSuccedded == false");

    for (int32 i = 0; i < 5; i++)
    {
        SCThreadManager::GetInstance().Launch([=]()
            {
                while (true)
                {
                    ServerService->GetIOCPCore()->Dispatch();
                }
            });
    }

    SCThreadManager::GetInstance().Join();

    SCSocketUtils::Clear();

    return 0;
}
