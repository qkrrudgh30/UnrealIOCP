#include "pch.h"
#include "SocketUtils.h"

LPFN_CONNECTEX SCSocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX SCSocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX SCSocketUtils::AcceptEx = nullptr;

void SCSocketUtils::Init()
{
	WSADATA WindowsSocketAPIData;
	int32 ResultCode = ::WSAStartup(MAKEWORD(2, 2), OUT & WindowsSocketAPIData);
	assert(ResultCode == 0 && "::WSAStartup(MAKEWORD(2, 2), OUT & WindowsSocketAPIData) != 0");

	SOCKET DummySocket = CreateSocket();
		// 더미 소켓을 하나 만들고 해당 소켓을 기준으로 함수를 호출.

	bool bIsConnectFunctionBound = BindWindowsFunction(DummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx));
	assert(bIsConnectFunctionBound == true && "BindWindowsFunction(DummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)) == true");
	bool bIsfAcceptFunctionBound = BindWindowsFunction(DummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx));
	assert(bIsfAcceptFunctionBound == true && "BindWindowsFunction(DummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)) == true");
	bool bIsfDisconnectFunctionBound = BindWindowsFunction(DummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx));
	assert(bIsfDisconnectFunctionBound == true && "BindWindowsFunction(DummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)) == false");

	Close(DummySocket);
}

SOCKET SCSocketUtils::CreateSocket()
{
	SOCKET sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (sock == INVALID_SOCKET)
	{
		int error = WSAGetLastError();  // 오류 코드 확인
		std::wcout << L"Socket creation failed. Error code: " << error << std::endl;
	}

	return sock;
}

bool SCSocketUtils::BindWindowsFunction(SOCKET InSocket, GUID InGUID, LPVOID* InCallback)
{
	DWORD Bytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(InSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &InGUID, sizeof(InGUID), InCallback, sizeof(*InCallback), OUT & Bytes, NULL, NULL);
}

bool SCSocketUtils::SetLinger(SOCKET InSocket, uint16 InOnOff, uint16 InLinger)
{
	LINGER LingerOption;
	LingerOption.l_onoff = InOnOff;
	LingerOption.l_linger = InLinger;
	return SetSocketOption(InSocket, SOL_SOCKET, SO_LINGER, LingerOption);
}

bool SCSocketUtils::SetReuseAddress(SOCKET InSocket, bool InFlag)
{
	return SetSocketOption(InSocket, SOL_SOCKET, SO_REUSEADDR, InFlag);
}

bool SCSocketUtils::SetRecvBufferSize(SOCKET InSocket, int32 InSize)
{
	return SetSocketOption(InSocket, SOL_SOCKET, SO_RCVBUF, InSize);
}

bool SCSocketUtils::SetSendBufferSize(SOCKET InSocket, int32 InSize)
{
	return SetSocketOption(InSocket, SOL_SOCKET, SO_SNDBUF, InSize);
}

bool SCSocketUtils::SetTcpNoDelay(SOCKET InSocket, bool InFlag)
{
	return SetSocketOption(InSocket, SOL_SOCKET, TCP_NODELAY, InFlag);
}

bool SCSocketUtils::Bind(SOCKET InSocket, SCNetAddress InNetAddress)
{
	return SOCKET_ERROR != ::bind(InSocket, reinterpret_cast<const SOCKADDR*>(&InNetAddress.GetSocketAddress()), sizeof(SOCKADDR_IN));
}

bool SCSocketUtils::BindAnyAddress(SOCKET InSocket, uint16 InPortNumber)
{
	SOCKADDR_IN MyAddress;
	MyAddress.sin_family = AF_INET;
	MyAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	MyAddress.sin_port = ::htons(InPortNumber);

	return SOCKET_ERROR != ::bind(InSocket, reinterpret_cast<const SOCKADDR*>(&MyAddress), sizeof(MyAddress));
}

bool SCSocketUtils::Listen(SOCKET InServerSocket, int32 InBackLog)
{
	return SOCKET_ERROR != ::listen(InServerSocket, InBackLog);
}

bool SCSocketUtils::SetUpdateAcceptSocket(SOCKET InClientSocket, SOCKET InServerSocket)
{
	// ServerSocket의 IPAddress, PortNumber, TCP_NODELAY 등의 속성들을 ClientSocket에도 적용되게끔 함.
	return SetSocketOption(InClientSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, InServerSocket);
}

void SCSocketUtils::Close(SOCKET& InSocket)
{
	if (InSocket != INVALID_SOCKET)
	{
		// 두번 close()하지 않기 위함.
		::closesocket(InSocket);
	}
	InSocket = INVALID_SOCKET;
}

void SCSocketUtils::Clear()
{
	::WSACleanup();
}
