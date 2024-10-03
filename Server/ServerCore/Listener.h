#pragma once

#include "IOCPCore.h"

class SCIOCPEvent;
class SCAcceptEvent;

class SCListener : public SCIOCPObject
{
public:
    SCListener() = default;

    ~SCListener();

public:
    bool StartAccept(SCNetAddress InNetAddress);

    void CloseSocket();

public:
    virtual HANDLE GetHandle() override;

    virtual void Dispatch(SCIOCPEvent* InIOCPEvent, int32 InNumberOfBytes = 0) override;

private:
    void ProcessAccept(SCAcceptEvent* InAcceptEvent);

    void RegisterAccept(SCAcceptEvent* InAcceptEvent);

protected:
    SOCKET ServerSocket = INVALID_SOCKET;

    std::vector<SCAcceptEvent*> AcceptEvents;

};
