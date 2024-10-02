#include "pch.h"

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
        int32 ErrorCode = ::WSAGetLastError();
        cout << "Socket ErrorCode : " << ErrorCode << endl;
        return 0;
    }

    int32 opt = 1;
    if (::setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == SOCKET_ERROR)
    {
        int32 ErrorCode = ::WSAGetLastError();
        cout << "SetSockOpt ErrorCode : " << ErrorCode << endl;
        return 0;
    }

    SOCKADDR_IN ServerAddress;
    ::memset(&ServerAddress, 0, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
    ServerAddress.sin_port = ::htons(8080);

    if (::bind(ServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
    {
        int32 ErrorCode = ::WSAGetLastError();
        cout << "Bind ErrorCode : " << ErrorCode << endl;
        return 0;
    }

    if (::listen(ServerSocket, 10) == SOCKET_ERROR)
    {
        int32 ErrorCode = ::WSAGetLastError();
        cout << "Listen ErrorCode : " << ErrorCode << endl;
        return 0;
    }

    while (true)
    {
        SOCKADDR_IN ClientAddress;
        ::memset(&ClientAddress, 0, sizeof(ClientAddress));
        int32 AddressSize = sizeof(ClientAddress);

        SOCKET ClientSocket = ::accept(ServerSocket, (SOCKADDR*)&ClientAddress, &AddressSize);
        if (ClientSocket == INVALID_SOCKET)
        {
            int32 ErrorCode = ::WSAGetLastError();
            cout << "Accept ErrorCode : " << ErrorCode << endl;
            return 0;
        }
    }

    ::WSACleanup();

    return 0;
}
