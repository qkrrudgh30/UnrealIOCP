#pragma once

class SCNetAddress;

//@brief: 소켓 프로그래밍 초반에 윈속 초기화 담당. 비동기 커넥트/억셉트/디스커넥트 함수의 경우 런타임에 주소를 얻어와야 함. 그런 작업들을 하는 클래스.
class SCSocketUtils
{
public:
    static void Init();

    static SOCKET CreateSocket();

    static bool BindWindowsFunction(SOCKET InSocket, GUID InGUID, LPVOID* InCallback);

    static bool SetLinger(SOCKET InSocket, uint16 InOnOff, uint16 InLinger);

    static bool SetReuseAddress(SOCKET InSocket, bool InFlag);

    static bool SetRecvBufferSize(SOCKET InSocket, int32 InSize);

    static bool SetSendBufferSize(SOCKET InSocket, int32 InSize);

    static bool SetTcpNoDelay(SOCKET InSocket, bool InFlag);

    static bool Bind(SOCKET InSocket, SCNetAddress InNetAddress);

    static bool BindAnyAddress(SOCKET InSocket, uint16 InPortNumber);

    static bool Listen(SOCKET InServerSocket, int32 InBackLog = SOMAXCONN);

    static bool SetUpdateAcceptSocket(SOCKET InClientSocket, SOCKET InServerSocket);

    static void Close(SOCKET& InSocket);

    static void Clear();

public:
    static LPFN_CONNECTEX ConnectEx;

    static LPFN_DISCONNECTEX DisconnectEx;

    static LPFN_ACCEPTEX AcceptEx;

};

template<typename T>
static inline bool SetSocketOption(SOCKET InSocket, int32 InLevel, int32 InOptionName, T InOptionValue)
{
    return SOCKET_ERROR != ::setsockopt(InSocket, InLevel, InOptionName, reinterpret_cast<char*>(&InOptionValue), sizeof(T));
}
