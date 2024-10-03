#pragma once

class SCNetAddress
{
public:
    SCNetAddress() = default;

    SCNetAddress(SOCKADDR_IN InSocketAddress);

    SCNetAddress(std::wstring InIPString, uint16 InPortNumber);

    SOCKADDR_IN& GetSocketAddress() { return SocketAddress; }

    std::wstring GetIPAddressWString();

    uint16 GetPort() { return ::ntohs(SocketAddress.sin_port); } 

public:
    static IN_ADDR	IP2Address(const WCHAR* InIPWCharString);

private:
    SOCKADDR_IN		SocketAddress = {};

};
