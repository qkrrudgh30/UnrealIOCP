#pragma once

//@brief: SOCKADDR_IN에 클라의 IP 주소를 받아줌. 서버 운영시 해당 클라의 IP를 추출을 매번하기 보다, 등록해서 관리하고자 함.
class SCNetAddress
{
public:
    SCNetAddress() = default;

    SCNetAddress(SOCKADDR_IN InSocketAddress);

    SCNetAddress(wstring InIPString, uint16 InPortNumber);
        // 서버의 모든 문자열은 wstring으로 통일.

    SOCKADDR_IN& GetSocketAddress() { return SocketAddress; }

    wstring	GetIPAddressWString();

    uint16 GetPort() { return ::ntohs(SocketAddress.sin_port); } 
        // network to host short.

public:
    static IN_ADDR	IP2Address(const WCHAR* InIPWCharString);

private:
    SOCKADDR_IN		SocketAddress = {};

};
