#include "pch.h"

void ThreadMain()
{
    while (true)
    {
        cout << "Hello, I'm thread" << LThreadID << endl;
        this_thread::sleep_for(1s);
    }
}

int main()
{
    for (uint32 i = 0; i < 5; ++i) 
    { 
        SCThreadManager::GetInstance().Launch(ThreadMain); 
    }

    return 0;
}
