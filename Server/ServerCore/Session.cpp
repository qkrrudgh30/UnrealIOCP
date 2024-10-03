#include "pch.h"
#include "Session.h"

SCSession::SCSession()
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
	//@TODO: 
}
