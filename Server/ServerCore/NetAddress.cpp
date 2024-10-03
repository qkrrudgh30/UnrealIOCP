#include "pch.h"
#include "NetAddress.h"

SCNetAddress::SCNetAddress(SOCKADDR_IN InSocketAddress)
	: SocketAddress(InSocketAddress)
{
}

SCNetAddress::SCNetAddress(wstring InIPString, uint16 InPortNumber)
{
    ::memset(&SocketAddress, 0, sizeof(SocketAddress));
    SocketAddress.sin_family = AF_INET;
    SocketAddress.sin_addr = IP2Address(InIPString.c_str());
    SocketAddress.sin_port = ::htons(InPortNumber);
}

wstring SCNetAddress::GetIPAddressWString()
{
    WCHAR IPAddressWCharString[100];
    ::InetNtopW(AF_INET, &SocketAddress.sin_addr, IPAddressWCharString, len32(IPAddressWCharString));
        
    return wstring(IPAddressWCharString);
}

IN_ADDR SCNetAddress::IP2Address(const WCHAR* InIPWCharString)
{
    IN_ADDR ResultAddress;
    ::InetPtonW(AF_INET, InIPWCharString, &ResultAddress);
    return ResultAddress;
}
