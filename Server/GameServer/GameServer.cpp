#include "pch.h"

int main()
{
    SCSocketUtils::Init();

    SOCKET ServerSocket = SCSocketUtils::CreateSocket();

    SCSocketUtils::BindAnyAddress(ServerSocket, 8080);

    SCSocketUtils::Listen(ServerSocket);

    SOCKET ClientSocket = ::accept(ServerSocket, nullptr, nullptr);

    //@TODO

    SCSocketUtils::Clear();

    return 0;
}
