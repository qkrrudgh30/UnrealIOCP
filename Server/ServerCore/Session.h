#pragma once

#include "IOCPCore.h"
#include "NetAddress.h"
#include "IOCPEvent.h"
#include "Service.h"
#include "RecvBuffer.h"

class SCSession : public SCIOCPObject
{
    enum { BUFFER_SIZE = 0x10000 }; // 64KB

public:
    SCSession();

    virtual ~SCSession();

public:
    void SetNetAddress(SCNetAddress InNetAddress) { NetAddress = InNetAddress; }

    SCNetAddress GetNetAddress() { return NetAddress; }

    SOCKET GetSocket() { return Socket; }

    SharedPtrSCSession GetSession() { return static_pointer_cast<SCSession>(shared_from_this()); }

    std::shared_ptr<SCService> GetOwnerService() { return OwnerService.lock(); }

    void SetOwnerService(std::shared_ptr<SCService> InOwnerService) { OwnerService = InOwnerService; }

public:
    virtual HANDLE GetHandle() override;

    virtual void Dispatch(SCIOCPEvent* InIOCPEvent, int32 InNumberOfBytes = 0) override;

    void HandleError(int32 InErrorCode);

#pragma region Connect

public:
    bool IsConnected() { return bIsConnected; }

    bool Connect();

protected:
    virtual void OnConnected() {}

private:
    bool RegisterConnect();

    void ProcessConnect();

#pragma endregion

#pragma region Receive

protected:
    virtual int32 OnRecv(BYTE* InBuffer, int32 InLength) { return InLength; }

private:
    void RegisterRecv();

    void ProcessRecv(int32 InLength);

#pragma endregion

#pragma region Send

public:
    void Send(SharedPtrSCSendBuffer InSendBuffer);

protected:
    virtual void OnSend(int32 InLength) {}

private:
    void RegisterSend();

    void ProcessSend(int32 InLength);

#pragma endregion

#pragma region Disconnect

public:
    void Disconnect(const WCHAR* InCause);

protected:
    virtual void OnDisconnected() {}

private:
    bool RegisterDisconnect();

    void ProcessDisconnect();

#pragma endregion

private:
    SOCKET Socket = INVALID_SOCKET;

    SCNetAddress NetAddress = {};

    std::atomic<bool> bIsConnected = false;

    std::weak_ptr<SCService> OwnerService;

    USE_LOCK;

    SCConnectEvent ConnectEvent;

    SCRecvEvent RecvEvent;

    SCSendEvent SendEvent;

    SCDisconnectEvent DisconnectEvent;

    std::queue<SharedPtrSCSendBuffer> SendBufferQueue;

    std::atomic<bool> bIsSendBufferRegistered = false;

public:
    //BYTE RecvBuffer[1000];
    SCRecvBuffer RecvBuffer;

};
