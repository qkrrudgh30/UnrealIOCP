#include "pch.h"
#include "ClientSession.h"

/*
class GSClientSession : public SCSession
{
public:
    ~GSClientSession()
    {
        cout << "~GSClientSession ()" << endl;
    }

    virtual int32 OnRecv(BYTE* InBuffer, int32 InLength) override
    {
        cout << "OnRecv Len = " << InLength << endl;

        SharedPtrSCSendBuffer SendBuffer = std::make_shared<SCSendBuffer>();
        SendBuffer->CopyData(InBuffer, InLength);
        Send(SendBuffer);

        return InLength;
    }

    virtual void OnSend(int32 InLength) override
    {
        cout << "OnSend Len = " << InLength << endl;
    }

};
*/

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

    SCThreadManager::GetInstance().Join();

    SCSocketUtils::Clear();

    return 0;
}
