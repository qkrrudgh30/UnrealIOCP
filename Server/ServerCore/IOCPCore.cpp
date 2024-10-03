#include "pch.h"
#include "IOCPCore.h"
#include "IOCPEvent.h"


SCIOCPCore::SCIOCPCore()
{
	IOCPHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	assert(IOCPHandle != INVALID_HANDLE_VALUE && "IOCPHandle == INVALID_HANDLE_VALUE");
}

SCIOCPCore::~SCIOCPCore()
{
	::CloseHandle(IOCPHandle);
}

bool SCIOCPCore::Register(SharedPtrSCIOCPObject InIOCPObject)
{
	return ::CreateIoCompletionPort(InIOCPObject->GetHandle(), IOCPHandle, 0, 0);
}

bool SCIOCPCore::Dispatch(uint32 InTimeoutMS)
{
    DWORD NumberOfBytes = 0;
    ULONG_PTR DummyCompletionKey = 0;
    SCIOCPEvent* IOCPEvent = nullptr;

    if (::GetQueuedCompletionStatus(IOCPHandle, OUT & NumberOfBytes, OUT &DummyCompletionKey, OUT reinterpret_cast<LPOVERLAPPED*>(&IOCPEvent), InTimeoutMS))
    {
        SharedPtrSCIOCPObject IOCPObject = IOCPEvent->OwnerIOCPObject;
        IOCPObject->Dispatch(IOCPEvent, NumberOfBytes);
    }
    else
    {
        int32 ErrorCode = ::WSAGetLastError();
        switch (ErrorCode)
        {
        case WAIT_TIMEOUT: 
            
            return false;
        default:
            SharedPtrSCIOCPObject IOCPObject = IOCPEvent->OwnerIOCPObject;
            IOCPObject->Dispatch(IOCPEvent, NumberOfBytes);
            break;
        }
    }

    return true;
}
