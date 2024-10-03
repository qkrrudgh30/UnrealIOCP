#include "pch.h"
#include "IOCPCore.h"
#include "IOCPEvent.h"

SCIOCPCore GIOCPCore;
	// 임시 코드

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
	return ::CreateIoCompletionPort(InIOCPObject->GetHandle(), IOCPHandle, 0, 0); // CompletionKey 매개변수에 0을 인자로 넘기는 모습.
}

bool SCIOCPCore::Dispatch(uint32 InTimeoutMS)
{
    DWORD NumberOfBytes = 0;
    ULONG_PTR DummyCompletionKey = 0; // Register() 함수에서도 0을 인자로 넘겼고, 등록할때도 0으로 등록. 즉, 사용하지 않겠다.
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
            // 10ms동안 일감을 기다려 달라했는데, 그걸 넘어버리면 GetQueuedCompletionStatus()에선 WAIT_TIMEOUT이 반환됨.
            return false;
        default:
            //@TODO : 로그 찍기
            SharedPtrSCIOCPObject IOCPObject = IOCPEvent->OwnerIOCPObject;
            IOCPObject->Dispatch(IOCPEvent, NumberOfBytes);
            break;
        }
    }

    return true;
}
