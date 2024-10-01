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

protected:
    static void InitTLS();
        // 이 클래스의 목적. TLS영역의 초기화와 날리는 함수를 만들어서 자동으로 해주기 위함.

    static void DestroyTLS();

    void Join(); 
        // 스레드들이 끝날 때까지 기다려주는 함수.

private:
    std::mutex Lock;

    std::vector<std::thread> Threads;

};
