#pragma once
#include"Job.hpp"
#include<queue>
#include<mutex>

namespace ShabbyThreadPool{

class Jobqueue{
    public:
        template<typename Func, typename ...Args> requires requires{Func();}
        void job_add(Func func, Args... args){
            std::lock_guard<std::mutex>queuelock(this->QueueMutex);
            this->JobQueue.push({Job(func, args...)});
        }
        void job_add(JobNodeBase && JNB){
            std::lock_guard<std::mutex>queuelock(this->QueueMutex);
            this->JobQueue.push(JNB);
        }
        void job_add(const JobNodeBase & JNB){
            std::lock_guard<std::mutex>queuelock(this->QueueMutex);
            this->JobQueue.push(JNB);
        }
        // template<typename T = JobNodeBase>
        // void job_add(T && t){
        //     std::lock_guard<std::mutex>queuelock(this->QueueMutex);
        //     this->JobQueue.push(std::forward<T>(t));
        // }
        JobNodeBase job_pop(){
            this->QueueMutex.lock();
            auto fornt = this->JobQueue.front();
            this->JobQueue.pop();
            this->QueueMutex.unlock();
            return fornt;
        }
        int job_num(){
            std::lock_guard<std::mutex>queuelock(this->QueueMutex);
            return this->JobQueue.size();
        }
        bool empty(){
            std::lock_guard<std::mutex>queuelock(this->QueueMutex);
            return this->JobQueue.empty();
        }
    private:
        std::queue<JobNodeBase>  JobQueue;
        std::mutex QueueMutex;
};

};