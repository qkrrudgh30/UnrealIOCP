#include "pch.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"

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
        SharedPtrSCSendBuffer BufferToSend = make_shared<SCSendBuffer>(4096);

        BYTE* Buffer = BufferToSend->GetBuffer();
        ((SCPacketHeader*)Buffer)->PacketSize = (sizeof(DataToSend) + sizeof(SCPacketHeader));
        ((SCPacketHeader*)Buffer)->PacketID = 1; // 1 : Hello Msg
        ::memcpy(&Buffer[4], DataToSend, sizeof(DataToSend));

        GSClientSessionManager::GetInstance().Broadcast(BufferToSend);

        this_thread::sleep_for(250ms);
    }

    SCThreadManager::GetInstance().Join();

    SCSocketUtils::Clear();

    return 0;
}
