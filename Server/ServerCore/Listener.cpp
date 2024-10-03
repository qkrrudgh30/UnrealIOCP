#include "pch.h"
#include "Listener.h"
#include "IOCPEvent.h"
#include "Session.h"

SCListener::~SCListener()
{
    SCSocketUtils::Close(ServerSocket);

    for (SCAcceptEvent* AcceptEvent : AcceptEvents)
    {
        //@TODO: 

        delete AcceptEvent;
        AcceptEvent = nullptr;
    }
}

bool SCListener::StartAccept(SCNetAddress InNetAddress)
{
    ServerSocket = SCSocketUtils::CreateSocket();
    if (ServerSocket == INVALID_SOCKET)
    {
        return false;
    }

    //if (GIOCPCore.Register(this) == false)
        // ������ �������� �ڵ�.
    if (GIOCPCore.Register(shared_from_this()) == false)
    {
        return false;
    }

    if (SCSocketUtils::SetReuseAddress(ServerSocket, true) == false)
    {
        return false;
    }

    if (SCSocketUtils::SetLinger(ServerSocket, 0, 0) == false)
    {
        return false;
    }

    if (SCSocketUtils::Bind(ServerSocket, InNetAddress) == false)
    {
        return false;
    }

    if (SCSocketUtils::Listen(ServerSocket) == false)
    {
        return false;
    }

    const int32 AcceptCount = 1;
    for (int32 i = 0; i < AcceptCount; i++)
    {
        SCAcceptEvent* AcceptEvent = new SCAcceptEvent();
        //AcceptEvent->OwnerIOCPObject = shared_ptr<SCIOCPObject>(this);
            // ����� �ۼ��ϸ� �ȵǴ� �ڵ�. ���� ī��Ʈ�� 1�� ���ο� shared_ptr ����� ��. ��, ������ raw pointer�� �� ������ shared_ptr�� �����ϴ� ��.
        AcceptEvent->OwnerIOCPObject = shared_from_this();
        AcceptEvents.push_back(AcceptEvent);
        RegisterAccept(AcceptEvent);
    }

    return true;
}

void SCListener::CloseSocket()
{
    SCSocketUtils::Close(ServerSocket);
}

HANDLE SCListener::GetHandle()
{
    return reinterpret_cast<HANDLE>(ServerSocket);
}

void SCListener::Dispatch(SCIOCPEvent* InIOCPEvent, int32 InNumberOfBytes)
{
    assert(InIOCPEvent->EventType == EEventType::Accept && "InIOCPEvent->GetEventType() == EEventType::Accept");
    SCAcceptEvent* AcceptEvent = static_cast<SCAcceptEvent*>(InIOCPEvent);
    ProcessAccept(AcceptEvent);
}

void SCListener::ProcessAccept(SCAcceptEvent* InAcceptEvent)
{
    SharedPtrSCSession ClientSession = InAcceptEvent->ClientSession;

    if (false == SCSocketUtils::SetUpdateAcceptSocket(ClientSession->GetSocket(), ServerSocket))
    {
        RegisterAccept(InAcceptEvent);
        return;
    }

    SOCKADDR_IN ClientSocketAddress;
    int32 ClientSocketAddressLength = sizeof(ClientSocketAddress);
    if (SOCKET_ERROR == ::getpeername(ClientSession->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&ClientSocketAddress), &ClientSocketAddressLength))
    {
        RegisterAccept(InAcceptEvent);
        return;
    }

    ClientSession->SetNetAddress(SCNetAddress(ClientSocketAddress));

    cout << "Client Connected!" << endl;

    //@TODO: 

    RegisterAccept(InAcceptEvent);
}

void SCListener::RegisterAccept(SCAcceptEvent* InAcceptEvent)
{
    SharedPtrSCSession ClientSession = std::make_shared<SCSession>();

    InAcceptEvent->Init();
    InAcceptEvent->ClientSession = ClientSession;

    DWORD ReceivedBytes = 0;
    if (false == SCSocketUtils::AcceptEx(ServerSocket, ClientSession->GetSocket(), ClientSession->RecvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & ReceivedBytes, static_cast<LPOVERLAPPED>(InAcceptEvent)))
    {
        const int32 ErrorCode = ::WSAGetLastError();
        if (ErrorCode != WSA_IO_PENDING)
        {
            // �ϴ� �ٽ� Accept �ɾ��ش�
            RegisterAccept(InAcceptEvent);
        }
    }
}
