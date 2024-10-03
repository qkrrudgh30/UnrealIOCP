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
        // ServerService의 부모 클래스인 Service 클래스에다가 enable_shared_from_this를 걸어줬기 때문에,
        // shared_from_this()의 반환값은 Service 자료형임. 따라서 형변환이 필요한데 shared_ptr 자료형의 형변환은 static_pointer_cast<>()
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
