#pragma once

#include <thread>
#include <functional>

class SCThreadManager
{
private:
    SCThreadManager();
    ~SCThreadManager();
    SCThreadManager(const SCThreadManager&) = delete;
    SCThreadManager& operator=(const SCThreadManager&) = delete;

public:
    static SCThreadManager& GetInstance();

    void Launch(std::function<void(void)> InCallback);

    void Join();

protected:
    static void InitTLS();
        // �� Ŭ������ ����. TLS������ �ʱ�ȭ�� ������ �Լ��� ���� �ڵ����� ���ֱ� ����.

    static void DestroyTLS();

private:
    std::mutex Lock;

    std::vector<std::thread> Threads;

};
