#include "pch.h"
#include "Session.h"

SCSession::SCSession()
	: RecvBuffer(BUFFER_SIZE)
{
	Socket = SCSocketUtils::CreateSocket();
}

SCSession::~SCSession()
{
	SCSocketUtils::Close(Socket);
}

HANDLE SCSession::GetHandle()
{
	return reinterpret_cast<HANDLE>(Socket);
}

void SCSession::Dispatch(SCIOCPEvent* InIOCPEvent, int32 InNumberOfBytes)
{
	switch (InIOCPEvent->EventType)
	{
	case EEventType::Connect:
		ProcessConnect();
		break;
	case EEventType::Recv:
		ProcessRecv(InNumberOfBytes);
		break;
	case EEventType::Send:
		//ProcessSend(static_cast<SCSendEvent*>(InIOCPEvent), InNumberOfBytes);
		ProcessSend(InNumberOfBytes);
		break;
	case EEventType::Disconnect:
		ProcessDisconnect();
		break;
	default:
		break;
	}
}

void SCSession::HandleError(int32 InErrorCode)
{
	switch (InErrorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TODO : Log
		cout << "Handle Error : " << InErrorCode << endl;
		break;
	}
}

bool SCSession::Connect()
{
	return RegisterConnect();
}

bool SCSession::RegisterConnect()
{
	/*
	ProcessConnect();

	return true;
	*/
	if (IsConnected() == true)
	{
		return false;
	}

	if (GetOwnerService()->GetServiceType() != EServiceType::Client)
	{
		return false;
	}

	if (SCSocketUtils::SetReuseAddress(Socket, true) == false)
	{
		return false;
	}

	if (SCSocketUtils::BindAnyAddress(Socket, 0) == false)
	{
		return false;
	}

	ConnectEvent.Init();
	ConnectEvent.OwnerIOCPObject = shared_from_this(); // ADD_REF

	DWORD NumberOfBytes = 0;
	SOCKADDR_IN ServerAddress = GetOwnerService()->GetNetAddress().GetSocketAddress();
	if (false == SCSocketUtils::ConnectEx(Socket, reinterpret_cast<SOCKADDR*>(&ServerAddress), sizeof(ServerAddress), nullptr, 0, &NumberOfBytes, &ConnectEvent))
	{
		int32 ErrorCode = ::WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			ConnectEvent.OwnerIOCPObject = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

void SCSession::ProcessConnect()
{
	ConnectEvent.OwnerIOCPObject = nullptr;

	bIsConnected.store(true);

	GetOwnerService()->AddSession(GetSession());

	OnConnected();

	RegisterRecv();
}

void SCSession::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	RecvEvent.Init();
	RecvEvent.OwnerIOCPObject = shared_from_this(); // ADD_REF

	WSABUF WindowsSocketAPIBuffer;
	//WindowsSocketAPIBuffer.buf = reinterpret_cast<char*>(RecvBuffer);
	//WindowsSocketAPIBuffer.len = len32(RecvBuffer);
	WindowsSocketAPIBuffer.buf = reinterpret_cast<char*>(RecvBuffer.GetCurrentWritePosition());
	WindowsSocketAPIBuffer.len = RecvBuffer.GetSlot();

	DWORD NumberOfBytes = 0;
	DWORD Flags = 0;
	if (SOCKET_ERROR == ::WSARecv(Socket, &WindowsSocketAPIBuffer, 1, OUT & NumberOfBytes, OUT & Flags, &RecvEvent, nullptr))
	{
		int32 ErrorCode = ::WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			HandleError(ErrorCode);
			RecvEvent.OwnerIOCPObject = nullptr; // RELEASE_REF
		}
	}
}

void SCSession::ProcessRecv(int32 InLength)
{
	RecvEvent.OwnerIOCPObject = nullptr; // RELEASE_REF

	//if (InLength == 0)
	if (RecvBuffer.OnWrite(InLength) == false)
	{
		Disconnect(L"SCRecvBuffer::OnWrite() overflow.");
		return;
	}

	int32 CountToRead = RecvBuffer.GetCountToRead();
	int32 ProcessLen = OnRecv(RecvBuffer.GetCurrentReadPosition(), CountToRead);
	if (ProcessLen < 0 || CountToRead < ProcessLen || RecvBuffer.OnRead(ProcessLen) == false)
	{
		Disconnect(L"SCRecvBuffer::OnRecv() overflow.");
		return;
	}

	cout << "Recv Data Len = " << InLength << endl;

	//OnRecv(RecvBuffer, InLength);
	RecvBuffer.Clean();

	RegisterRecv();
}

void SCSession::Send(SharedPtrSCSendBuffer InSendBuffer)
{
	if (IsConnected() == false)
	{
		return;
	}

	bool bIsRegisterSend = false;

	{
		WRITE_LOCK;

		SendBufferQueue.push(InSendBuffer);

		if (bIsSendBufferRegistered.exchange(true) == false)
		{
			bIsRegisterSend = true;
		}

		if (bIsRegisterSend == true)
		{
			RegisterSend();
		}
	}
}

void SCSession::RegisterSend()
{
	if (IsConnected() == false)
		return;

	SendEvent.Init();
	SendEvent.OwnerIOCPObject = shared_from_this(); // ADD_REF

	{
		WRITE_LOCK;

		int32 TotalWriteCount = 0;
		while (SendBufferQueue.empty() == false)
		{
			SharedPtrSCSendBuffer SendBuffer = SendBufferQueue.front();

			TotalWriteCount += SendBuffer->GetCount();

			SendBufferQueue.pop();
			SendEvent.SendBuffers.push_back(SendBuffer);
		}
	}

	//WSABUF wsaBuf;
	//wsaBuf.buf = (char*)InSendEvent->SendBuffer.data();
	//wsaBuf.len = (ULONG)InSendEvent->SendBuffer.size();
	std::vector<WSABUF> wsaBufs;
	wsaBufs.reserve(SendEvent.SendBuffers.size());
	for (SharedPtrSCSendBuffer SendBuffer : SendEvent.SendBuffers)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(SendBuffer->GetBuffer());
		wsaBuf.len = static_cast<LONG>(SendBuffer->GetCount());
		wsaBufs.push_back(wsaBuf);
	}

	DWORD numOfBytes = 0;
	//if (SOCKET_ERROR == ::WSASend(Socket, &wsaBuf, 1, OUT & numOfBytes, 0, InSendEvent, nullptr))
	if (SOCKET_ERROR == ::WSASend(Socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), OUT & numOfBytes, 0, &SendEvent, nullptr))
	{
		int32 ErrorCode = ::WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			HandleError(ErrorCode);
			//InSendEvent->OwnerIOCPObject = nullptr; // RELEASE_REF
			SendEvent.OwnerIOCPObject = nullptr; // RELEASE_REF
			//xdelete(InSendEvent);
			SendEvent.SendBuffers.clear();
			bIsSendBufferRegistered.store(false);
		}
	}
}

void SCSession::ProcessSend(int32 InLength)
{
	//InSendEvent->OwnerIOCPObject = nullptr; // RELEASE_REF
	//delete InSendEvent;
	//InSendEvent = nullptr;
	SendEvent.OwnerIOCPObject = nullptr; // RELEASE_REF
	SendEvent.SendBuffers.clear();


	if (InLength == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	OnSend(InLength);

	WRITE_LOCK;
	if (SendBufferQueue.empty())
		bIsSendBufferRegistered.store(false);
	else
		RegisterSend();
}

void SCSession::Disconnect(const WCHAR* InCause)
{
	if (bIsConnected.exchange(false) == false)
	{
		return;
	}

	wcout << "Disconnect : " << InCause << endl;

	OnDisconnected();
	SCSocketUtils::Close(Socket);
	GetOwnerService()->ReleaseSession(GetSession());

	RegisterDisconnect();
}

bool SCSession::RegisterDisconnect()
{
	DisconnectEvent.Init();
	DisconnectEvent.OwnerIOCPObject = shared_from_this(); // ADD_REF

	if (false == SCSocketUtils::DisconnectEx(Socket, &DisconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 ErrorCode = ::WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			DisconnectEvent.OwnerIOCPObject = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

void SCSession::ProcessDisconnect()
{
	DisconnectEvent.OwnerIOCPObject = nullptr; // RELEASE_REF
}
