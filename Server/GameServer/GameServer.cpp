#include "pch.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "ClientPacketHandler.h"

int main()
{
    SCSocketUtils::Init();

    SharedPtrSCServerService ServerService = std::make_shared<SCServerService>(
        SCNetAddress(L"127.0.0.1", 8080),
        std::make_shared<SCIOCPCore>(),
        [=]() -> SharedPtrSCSession { return std::make_shared<GSClientSession>(); },
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

    char DataToSend[1000] = "Hello, Client!";

    while (true)
    {
        SharedPtrSCSendBuffer SendBuffer = GSClientPacketHandler::Make_S_TEST(7);
        
        GSClientSessionManager::GetInstance().Broadcast(SendBuffer);

        this_thread::sleep_for(1s);
    }

    SCThreadManager::GetInstance().Join();

    SCSocketUtils::Clear();

    return 0;
}
