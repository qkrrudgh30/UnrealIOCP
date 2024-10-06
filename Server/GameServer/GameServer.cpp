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
        //SharedPtrSCSendBuffer BufferToSend = make_shared<SCSendBuffer>(4096);
        //SCWritableBuffer WritableBuffer(BufferToSend->GetBuffer(), 4096);
        //
        //SCPacketHeader* PacketHeader = WritableBuffer.Reserve<SCPacketHeader>();
        //// ID(uint64), HP(uint32), Attack(uint16)
        //WritableBuffer << (uint64)1001 << (uint32)100 << (uint16)10;
        //WritableBuffer.Write(DataToSend, sizeof(DataToSend));
        //
        //PacketHeader->PacketSize = WritableBuffer.GetCurrentWriteIndex();
        //PacketHeader->PacketID = 1;
        //
        //GSClientSessionManager::GetInstance().Broadcast(BufferToSend);
        //
        //this_thread::sleep_for(250ms);

        vector<GSVector> Transform{ GSVector{ 100.f, 100.f, 0.f }, GSVector{ 0.f, 0.f, 0.f }, GSVector{ 1.f, 1.f, 1.f } };
        SharedPtrSCSendBuffer SendBuffer = GSClientPacketHandler::Make_S_TEST(7, Transform);

        GSClientSessionManager::GetInstance().Broadcast(SendBuffer);

        this_thread::sleep_for(250ms);
    }

    SCThreadManager::GetInstance().Join();

    SCSocketUtils::Clear();

    return 0;
}
