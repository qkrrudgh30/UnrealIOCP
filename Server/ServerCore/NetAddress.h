#pragma once

//@brief: SOCKADDR_IN�� Ŭ���� IP �ּҸ� �޾���. ���� ��� �ش� Ŭ���� IP�� ������ �Ź��ϱ� ����, ����ؼ� �����ϰ��� ��.
class SCNetAddress
{
public:
    SCNetAddress() = default;

    SCNetAddress(SOCKADDR_IN InSocketAddress);

    SCNetAddress(wstring InIPString, uint16 InPortNumber);
        // ������ ��� ���ڿ��� wstring���� ����.

    SOCKADDR_IN& GetSocketAddress() { return SocketAddress; }

    wstring	GetIPAddressWString();

    uint16 GetPort() { return ::ntohs(SocketAddress.sin_port); } 
        // network to host short.

public:
    static IN_ADDR	IP2Address(const WCHAR* InIPWCharString);

private:
    SOCKADDR_IN		SocketAddress = {};

};
