#include "pch.h"

enum class EIOType
{
    None,
    Accept,
    Connect,
    Read,
    Write,
    Disconnect,
};

struct GSSession
{
    SOCKET Socket = INVALID_SOCKET;

    char RecvBuffer[1000] = {};

    int32 RecvBufferLength = 0;

};

struct GSExtendedOverlapped
{
    WSAOVERLAPPED Overlapped = {};

    EIOType IOType = EIOType::None;

};

void HandleError(const char* cause)
{
    int32 ErrorCode = ::WSAGetLastError();
    cout << cause << " ErrorCode : " << ErrorCode << endl;
}

void ProcessRecv(HANDLE IOCPHandle)
{
    while (true)
    {
        DWORD TransferredBytes = 0;
        GSSession* Session = nullptr;
        GSExtendedOverlapped* ExtendedOverlapped = nullptr;

        BOOL ReturnedStatus = ::GetQueuedCompletionStatus(
            IOCPHandle,
            &TransferredBytes,
            (ULONG_PTR*)&Session,
            (LPOVERLAPPED*)&ExtendedOverlapped,
            INFINITE
        );

        if (ReturnedStatus == FALSE || TransferredBytes == 0)
        {
            HandleError("GetQueuedCompletionStatus()");
            continue;
        }

        assert(ExtendedOverlapped->IOType == EIOType::Read && "ExtendedOverlapped->IOType != EIOType::Read");

        cout << "Recv Data IOCP = " << TransferredBytes << endl;

        WSABUF WindowsSocketAPIBuffer;
        WindowsSocketAPIBuffer.buf = Session->RecvBuffer;
        WindowsSocketAPIBuffer.len = 1000;

        DWORD ReceiveLength = 0;
        DWORD Flags = 0;
        ::WSARecv(Session->Socket, &WindowsSocketAPIBuffer, 1, &ReceiveLength, &Flags, &ExtendedOverlapped->Overlapped, NULL);
    }
}

int main()
{
    WSAData WindowsSocketAPIData;
    if (::WSAStartup(MAKEWORD(2, 2), &WindowsSocketAPIData) != 0)
    {
        return 0;
    }

    SOCKET ServerSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocket == INVALID_SOCKET)
    {
        HandleError("socket()");
        return 0;
    }

    SOCKADDR_IN ServerAddress;
    ::memset(&ServerAddress, 0, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
    ServerAddress.sin_port = ::htons(8080);

    if (::bind(ServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
    {
        HandleError("bind()");
        return 0;
    }

    if (::listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        HandleError("listen()");
        return 0;
    }

    cout << "Server is now listening!" << endl;

    vector<GSSession*> SessionManager;

    HANDLE IOCPHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    for (int32 i = 0; i < 5; i++)
    {
        SCThreadManager::GetInstance().Launch(
            [=]() -> void
            {
                ProcessRecv(IOCPHandle);
            }
        );
    }

    while (true)
    {
        SOCKADDR_IN ClientAddress;
        int32 AddressLength = sizeof(ClientAddress);

        SOCKET ClientSocket = ::accept(ServerSocket, (SOCKADDR*)&ClientAddress, &AddressLength);
        if (ClientSocket == INVALID_SOCKET)
        {
            return 0;
        }

        GSSession* ClientSession = new GSSession();
        ClientSession->Socket = ClientSocket;
        SessionManager.push_back(ClientSession);

        cout << "Client Connected !" << endl;

        ::CreateIoCompletionPort((HANDLE)ClientSocket, IOCPHandle, (ULONG_PTR)ClientSession, 0);
        
        WSABUF WindowsSocketAPIBuffer;
        WindowsSocketAPIBuffer.buf = ClientSession->RecvBuffer;
        WindowsSocketAPIBuffer.len = 1000;

        GSExtendedOverlapped* OverlappedEx = new GSExtendedOverlapped();
        OverlappedEx->IOType = EIOType::Read;

        DWORD ReceiveLength = 0;
        DWORD Flags = 0;
        ::WSARecv(ClientSocket, &WindowsSocketAPIBuffer, 1, &ReceiveLength, &Flags, &OverlappedEx->Overlapped, NULL);
    }

    ::WSACleanup();

    return 0;
}
