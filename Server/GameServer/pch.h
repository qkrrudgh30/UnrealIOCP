// pch.h

#pragma once

// �Ʒ� �ڵ�� ������Ʈ �Ӽ� > ��Ŀ > �Է� > �߰� ���Ӽ��� �߰��ϴ� �Ͱ� ������ ȿ��. �߰� ���Ӽ��� ServerCore.lib�� �־�ǳ�, �װ� ��� �Ʒ� �ڵ带 �ۼ��ص���.
#ifdef _DEBUG
	#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#else 
	#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#endif // _DEBUG

#include "CorePCH.h"
