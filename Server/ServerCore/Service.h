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
    SCService(EServiceType InServiceType/*����Ÿ��. �������� Ŭ������*/, SCNetAddress InNetAddress/*������� �ڱ��ڽ��� InNetAddress, Ŭ���� ���� �پ���� ��� �ּ�*/,
        SharedPtrSCIOCPCore InIOCPCore/*�ھ �ϳ����� ���� or �ھ �и��ؼ� ť�� ������ �Ҽ���*/,
        SCSessionFactory InSessionFactory/*���� ������ִ� �Լ�*/, int32 InMaxSessionCount = 1/*�ִ� ������. 1:1�� �Ұ�� 1*/);

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
