#pragma once

#define OUT

using BYTE = unsigned   char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#define size16(val)		static_cast<int16>(sizeof(val))
#define size32(val)		static_cast<int32>(sizeof(val))
#define len16(arr)		static_cast<int16>(sizeof(arr)/sizeof(arr[0]))
#define len32(arr)		static_cast<int32>(sizeof(arr)/sizeof(arr[0]))
	// sizeof(buffer)�� ������, wchar[]�� ���Ҵ� 2����Ʈ ũ��� ����� ��ȯ �ȵɼ��� ����.
	// �׷��� ���� len32() ��ũ�� �Լ��� ������.

#define USING_SHARED_PTR(InClassName) using SharedPtr##InClassName = std::shared_ptr<class InClassName>;

USING_SHARED_PTR(SCIOCPCore);
USING_SHARED_PTR(SCIOCPObject);
USING_SHARED_PTR(SCSession);
USING_SHARED_PTR(SCListener);
USING_SHARED_PTR(SCServerService);
USING_SHARED_PTR(SCClientService);
USING_SHARED_PTR(SCSendBuffer);
USING_SHARED_PTR(SCPacketSession);
