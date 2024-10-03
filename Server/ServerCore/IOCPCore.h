#pragma once

class SCIOCPEvent;

class SCIOCPObject : public std::enable_shared_from_this<SCIOCPObject>
{
public:
	virtual HANDLE GetHandle() abstract;

	virtual void Dispatch(SCIOCPEvent* InIOCPEvent, int32 InNumberOfBytes = 0) abstract;	

};

class SCIOCPCore
{
public:
	SCIOCPCore();

	~SCIOCPCore();

	HANDLE GetHandle() { return IOCPHandle; }

	bool Register(SharedPtrSCIOCPObject InIOCPObject);

	bool Dispatch(uint32 InTimeoutMS = INFINITE);

private:
	HANDLE IOCPHandle;

};

extern SCIOCPCore GIOCPCore;
	// 임시 코드
