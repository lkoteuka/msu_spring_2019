#include <thread>
#include <iostream>
#include <atomic>

std::atomic<int> i;

void t1()
{
    while(i < 1000000)
    {
        if(i % 2 == 1)
        {
            std::cout << "pong" << std::endl;
            i++;
        }
    }
}

void t2()
{
    while(i < 1000000)
    {
        if(i % 2 == 0)
        {
            std::cout << "ping" << std::endl;
            i++;
        }
    }
}

int main()
{
    i = 0;
    std::thread thr1(t1);
    std::thread thr2(t2);
    thr1.join();
    thr2.join();
    return 0;
}