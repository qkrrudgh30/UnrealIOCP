#include "pch.h"

int main()
{
    SCSocketUtils::Init();

    SharedPtrSCListener Listener = std::make_shared<SCListener>();
    Listener->StartAccept(SCNetAddress(L"127.0.0.1", 8080));

    // �������� ������ core ���� Ȥ�� 1.5�� ������ ����. �׺��� �������� context switching�� �Ͼ�� ����.
    for (int32 i = 0; i < 5; i++)
    {
        SCThreadManager::GetInstance().Launch([=]()
            {
                while (true)
                {
                    GIOCPCore.Dispatch();
                }
            });
    }

    SCThreadManager::GetInstance().Join();

    SCSocketUtils::Clear();

    return 0;
}
