#include "pch.h"
#include "SocketUtils.h"
#include "NetAddress.h"

LPFN_CONNECTEX SCSocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX SCSocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX SCSocketUtils::AcceptEx = nullptr;

void SCSocketUtils::Init()
{
	WSADATA WindowsSocketAPIData;
	assert(::WSAStartup(MAKEWORD(2, 2), OUT & WindowsSocketAPIData) == 0 & "::WSAStartup(MAKEWORD(2, 2), OUT & WindowsSocketAPIData) != 0");

	SOCKET DummySocket = CreateSocket();
		// 더미 소켓을 하나 만들고 해당 소켓을 기준으로 함수를 호출.
	assert(BindWindowsFunction(DummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)) == true &
			"BindWindowsFunction(DummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)) == true");
	assert(BindWindowsFunction(DummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)) == true & 
			"BindWindowsFunction(DummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)) == true");
	assert(BindWindowsFunction(DummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)) == true & 
			"BindWindowsFunction(DummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)) == false");

	Close(DummySocket);
}

SOCKET SCSocketUtils::CreateSocket()
{
	// 임의의 TCP 소켓을 만드는 함수.
	// WSASocket() 함수를 활용하면 좀 더 자세한 옵션을 설정할 수 있음.
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED/*overlapped 함수들을 사용가능해짐.*/);
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
