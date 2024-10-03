#include "pch.h"
#include "IOCPEvent.h"

SCIOCPEvent::SCIOCPEvent(EEventType InEventType)
    : EventType(InEventType)
{
    Init();
}

void SCIOCPEvent::Init()
{
    OVERLAPPED::hEvent = 0;
    OVERLAPPED::Internal = 0;
    OVERLAPPED::InternalHigh = 0;
    OVERLAPPED::Offset = 0;
    OVERLAPPED::OffsetHigh = 0;
}
