#include"ShabbyThreadPool.hpp"
#include<atomic>

std::atomic_uint64_t val = 0;

int main(){
    ShabbyThreadPool::ThreadPool thread_pool(20);
    auto start = std::chrono::system_clock::now();
    auto func = [](){
        for(int i = 0;i<1000; i++)
        val.fetch_add(1);
    };
    for(int i = 0; i<1000000; i++){
        thread_pool.job_add(func);
    }
    thread_pool.detach_run();
    while(val<1000'000'000){
        std::this_thread::yield();
    }
    auto end = std::chrono::system_clock::now();
    std::cout<<"complete and wasted:"<<(end-start).count()<<std::endl;
    return 0;
}
