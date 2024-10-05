#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <atomic>
#include <mutex>

#define _WINSOCK_DEPRECATED_NO_WARNINGS // winsock c4996 Ã³¸®

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

#include <windows.h>
#include <cassert>

#include "Types.h"
#include "CoreTLS.h"
#include "CoreMacro.h"
#include "ThreadManager.h"
#include "NetAddress.h"
#include "SocketUtils.h"
#include "Listener.h"
#include "Session.h"
#include "Service.h"
#include "RecvBuffer.h"
#include "SendBuffer.h"
#include "ReadableBuffer.h"
#include "WritableBuffer.h"

using namespace std;
