#pragma once
#include"Job_queue.hpp"
#include<atomic>
#include<thread>
#include"Excutor.hpp"

namespace ShabbyThreadPool{

class ThreadPool{
    public:
        Jobqueue jqueue;
        int thread_num;
        std::thread *thread_pool;
    public:
        ThreadPool(int num){
            if(num<1){
                thread_num = 1;
            }
            else this->thread_num = num;
        }
        int job_num(){
            return jqueue.job_num();
        }
        template<typename Func, typename ...Args>
        void job_add(Func func, Args ...args){
            this->jqueue.job_add(Job(func,args...));
        }
        template<typename T>
        void job_add(T && t){
            this->jqueue.job_add(Job(std::forward<T>(t)));
        }
        void detach_run(){
            std::thread thr(&ThreadPool::thread_run, this);
            thr.detach();
        }
    private:
        void thread_run(){
            Excutor ext(&jqueue);
            this->thread_pool = new std::thread[thread_num];
            for(int i = 0; i<thread_num; i++){
                thread_pool[i] = std::thread(ext);
                thread_pool[i].detach();
            }
            delete[] thread_pool;
        }
};

};