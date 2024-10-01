// pch.h

#pragma once

// 아래 코드는 프로젝트 속성 > 링커 > 입력 > 추가 종속성에 추가하는 것과 동일한 효과. 추가 종속성에 ServerCore.lib를 넣어도되나, 그거 대신 아래 코드를 작성해도됨.
#ifdef _DEBUG
	#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#else 
	#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#endif // _DEBUG

#include "CorePCH.h"
