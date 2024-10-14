#include "pch.h"
#include "Listener.h"
#include "IOCPEvent.h"
#include "Session.h"
#include "Service.h"

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

bool SCListener::StartAccept(SharedPtrSCServerService InServerService)
{
    ServerService = InServerService;
    if (ServerService == nullptr)
    {
        return false;
    }

    ServerSocket = SCSocketUtils::CreateSocket();
    if (ServerSocket == INVALID_SOCKET)
    {
        return false;
    }

    //if (GIOCPCore.Register(shared_from_this()) == false)
    if (ServerService->GetIOCPCore()->Register(shared_from_this()) == false)
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

    //if (SCSocketUtils::Bind(ServerSocket, InNetAddress) == false)
    if (SCSocketUtils::Bind(ServerSocket, ServerService->GetNetAddress()) == false)
    {
        return false;
    }

    if (SCSocketUtils::Listen(ServerSocket) == false)
    {
        return false;
    }

    //const int32 AcceptCount = 1;
    const int32 AcceptCount = ServerService->GetMaxSessionCount();
    for (int32 i = 0; i < AcceptCount; i++)
    {
        SCAcceptEvent* AcceptEvent = new SCAcceptEvent();
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
    ClientSession->ProcessConnect();
    //cout << "Client Connected!" << endl;

    //@TODO: 

    RegisterAccept(InAcceptEvent);
}

void SCListener::RegisterAccept(SCAcceptEvent* InAcceptEvent)
{
    SharedPtrSCSession ClientSession = ServerService->CreateSession();

    InAcceptEvent->Init();
    InAcceptEvent->ClientSession = ClientSession;

    DWORD ReceivedBytes = 0;
    //if (false == SCSocketUtils::AcceptEx(ServerSocket, ClientSession->GetSocket(), ClientSession->RecvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & ReceivedBytes, static_cast<LPOVERLAPPED>(InAcceptEvent)))
    if (false == SCSocketUtils::AcceptEx(ServerSocket, ClientSession->GetSocket(), ClientSession->RecvBuffer.GetCurrentWritePosition(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & ReceivedBytes, static_cast<LPOVERLAPPED>(InAcceptEvent)))
    {
        const int32 ErrorCode = ::WSAGetLastError();
        if (ErrorCode != WSA_IO_PENDING)
        {
            RegisterAccept(InAcceptEvent);
        }
    }
}
