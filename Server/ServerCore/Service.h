#pragma once

#include "NetAddress.h"

enum class EServiceType : uint8
{
	None,
	Server,
	Client,
};

using SCSessionFactory = std::function<SharedPtrSCSession(void)>;

class SCService : public std::enable_shared_from_this<SCService>
{
public:
    SCService(EServiceType InServiceType/*서비스타입. 서버인지 클라인지*/, SCNetAddress InNetAddress/*서버라면 자기자신의 InNetAddress, 클라라면 내가 붙어야할 상대 주소*/,
        SharedPtrSCIOCPCore InIOCPCore/*코어를 하나만들어서 공유 or 코어를 분리해서 큐를 여러개 할수도*/,
        SCSessionFactory InSessionFactory/*세션 만들어주는 함수*/, int32 InMaxSessionCount = 1/*최대 동접수. 1:1로 할경우 1*/);

    virtual ~SCService();

    virtual bool Start() abstract;

    void SetSessionFactory(SCSessionFactory InSessionFactory) { SessionFactory = InSessionFactory; }

    bool CanStart() { return SessionFactory != nullptr; }

    virtual void Close();

    SharedPtrSCSession CreateSession();

    void AddSession(SharedPtrSCSession InSession);

    void ReleaseSession(SharedPtrSCSession InSession);

    int32 GetCurrentSessionCount() { return CurrentSessionCount; }

    int32 GetMaxSessionCount() { return MaxSessionCount; }

public:
    EServiceType GetServiceType() { return ServiceType; }

    SCNetAddress GetNetAddress() { return NetAddress; }

    SharedPtrSCIOCPCore& GetIOCPCore() { return IOCPCore; }

protected:
    USE_LOCK;

    EServiceType ServiceType = EServiceType::None;

    SCNetAddress NetAddress = {};

    SharedPtrSCIOCPCore IOCPCore;

    std::set<SharedPtrSCSession> Sessions;

    int32 CurrentSessionCount = 0;

    int32 MaxSessionCount = 0;

    SCSessionFactory SessionFactory;

};

class SCServerService : public SCService
{
public:
    SCServerService(SCNetAddress InServerAddress, SharedPtrSCIOCPCore InIOCPCore, SCSessionFactory InServerSessionFactory, int32 InMaxSessionCount = 1);

    virtual ~SCServerService() {}

    virtual bool Start() override;

    virtual void Close() override;

private:
    SharedPtrSCListener Listener = nullptr;

};

class SCClientService : public SCService
{
public:
    SCClientService(SCNetAddress InServerAddress, SharedPtrSCIOCPCore InIOCPCore, SCSessionFactory InClientSessionFactory, int32 InMaxSessionCount = 1);

    virtual ~SCClientService() {}

    virtual bool Start() override;

    virtual void Close() override;

};
