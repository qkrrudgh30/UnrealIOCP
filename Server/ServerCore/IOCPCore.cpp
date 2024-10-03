#include "pch.h"
#include "IOCPCore.h"
#include "IOCPEvent.h"

SCIOCPCore GIOCPCore;
	// �ӽ� �ڵ�

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
	return ::CreateIoCompletionPort(InIOCPObject->GetHandle(), IOCPHandle, 0, 0); // CompletionKey �Ű������� 0�� ���ڷ� �ѱ�� ���.
}

bool SCIOCPCore::Dispatch(uint32 InTimeoutMS)
{
    DWORD NumberOfBytes = 0;
    ULONG_PTR DummyCompletionKey = 0; // Register() �Լ������� 0�� ���ڷ� �Ѱ��, ����Ҷ��� 0���� ���. ��, ������� �ʰڴ�.
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
            // 10ms���� �ϰ��� ��ٷ� �޶��ߴµ�, �װ� �Ѿ������ GetQueuedCompletionStatus()���� WAIT_TIMEOUT�� ��ȯ��.
            return false;
        default:
            //@TODO : �α� ���
            SharedPtrSCIOCPObject IOCPObject = IOCPEvent->OwnerIOCPObject;
            IOCPObject->Dispatch(IOCPEvent, NumberOfBytes);
            break;
        }
    }

    return true;
}
