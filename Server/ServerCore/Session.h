#pragma once

#include "IOCPCore.h"
#include "NetAddress.h"

class SCIOCPEvent;

class SCSession : public SCIOCPObject
{
public:
    SCSession();

    virtual ~SCSession();

public:
    void SetNetAddress(SCNetAddress InNetAddress) { NetAddress = InNetAddress; }

    SCNetAddress GetNetAddress() { return NetAddress; }

    SOCKET GetSocket() { return Socket; }

public:
    virtual HANDLE GetHandle() override;

    virtual void Dispatch(SCIOCPEvent* InIOCPEvent, int32 InNumberOfBytes = 0) override;

public:
    char RecvBuffer[1000];
        // 임시 코드

private:
    SOCKET Socket = INVALID_SOCKET;

    SCNetAddress NetAddress = {};

    std::atomic<bool> bIsConnected = false;

};
