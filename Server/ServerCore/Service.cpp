#include "pch.h"
#include "Service.h"
#include "Session.h"

SCService::SCService(EServiceType InServiceType, SCNetAddress InNetAddress, SharedPtrSCIOCPCore InIOCPCore, SCSessionFactory InSessionFactory, int32 InMaxSessionCount)
    : ServiceType(InServiceType)
    , NetAddress(InNetAddress)
    , IOCPCore(InIOCPCore)
    , SessionFactory(InSessionFactory)
    , MaxSessionCount(InMaxSessionCount)
{

}

SCService::~SCService()
{
}

bool SCService::Start()
{
    //@TODO
    return true;
}

void SCService::Close()
{
    //@TODO
}

SharedPtrSCSession SCService::CreateSession()
{
    SharedPtrSCSession Session = SessionFactory();
    Session->SetOwnerService(shared_from_this());

    if (IOCPCore->Register(Session) == false)
    {
        return nullptr;
    }

    return Session;
}

void SCService::AddSession(SharedPtrSCSession InSession)
{
    WRITE_LOCK;
    ++CurrentSessionCount;
    Sessions.insert(InSession);
}

void SCService::ReleaseSession(SharedPtrSCSession InSession)
{
    WRITE_LOCK;
    size_t ErasedCount = Sessions.erase(InSession);
    assert(ErasedCount != 0 && "ErasedCount == 0");
    --CurrentSessionCount;
}

SCServerService::SCServerService(SCNetAddress InServerAddress, SharedPtrSCIOCPCore InIOCPCore, SCSessionFactory InServerSessionFactory, int32 InMaxSessionCount)
    : SCService(EServiceType::Server, InServerAddress, InIOCPCore, InServerSessionFactory, InMaxSessionCount)
{
}

bool SCServerService::Start()
{
    SCService::Start();

    if (CanStart() == false)
    {
        return false;
    }

    Listener = std::make_shared<SCListener>();
    if (Listener == nullptr)
    {
        return false;
    }

    SharedPtrSCServerService ServerService = static_pointer_cast<SCServerService>(shared_from_this());
        // ServerService�� �θ� Ŭ������ Service Ŭ�������ٰ� enable_shared_from_this�� �ɾ���� ������,
        // shared_from_this()�� ��ȯ���� Service �ڷ�����. ���� ����ȯ�� �ʿ��ѵ� shared_ptr �ڷ����� ����ȯ�� static_pointer_cast<>()
    if (Listener->StartAccept(ServerService) == false)
    {
        return false;
    }

    return true;
}

void SCServerService::Close()
{
    //@TODO

    SCService::Close();
}

SCClientService::SCClientService(SCNetAddress InServerAddress, SharedPtrSCIOCPCore InIOCPCore, SCSessionFactory InClientSessionFactory, int32 InMaxSessionCount)
    : SCService(EServiceType::Client, InServerAddress, InIOCPCore, InClientSessionFactory, InMaxSessionCount)
{
}

bool SCClientService::Start()
{
    SCService::Start();

    if (CanStart() == false)
    {
        return false;
    }

    const int32 MaxSessionCount = GetMaxSessionCount();
    for (int32 i = 0; i < MaxSessionCount; i++)
    {
        SharedPtrSCSession ServerSession = CreateSession();
        if (ServerSession->Connect() == false)
        {
            return false;
        }
    }

    return true;
}

void SCClientService::Close()
{
    //@TODO

    SCService::Close();
}
