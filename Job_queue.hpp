#pragma once
#include"Job.hpp"
#include<queue>
#include<mutex>
#include<shared_mutex>

namespace ShabbyThreadPool{

class Jobqueue{
    public:
        template<typename Func, typename ...Args> requires requires{Func();}
        void job_add(Func func, Args... args){
            std::lock_guard<std::mutex>queuelock(this->write_QueueMutex);
            this->JobQueue.push({Job(func, args...)});
        }
        template<typename T = JobNodeBase>
        void job_add(T && t){
            std::lock_guard<std::mutex>queuelock(this->write_QueueMutex);
            this->JobQueue.push(Job(std::forward<T>(t)));
        }
        JobNodeBase job_pop(){
            this->write_QueueMutex.lock();
            auto fornt = this->JobQueue.front();
            this->JobQueue.pop();
            this->write_QueueMutex.unlock();
            return fornt;
        }
        int job_num(){
            std::lock_guard<std::shared_mutex>queuelock(this->read_QueueMutex);
            return this->JobQueue.size();
        }
        bool empty(){
            std::lock_guard<std::mutex>queuelock(this->write_QueueMutex);
            return this->JobQueue.empty();
        }
    private:
        std::queue<JobNodeBase>  JobQueue;
        std::mutex pop_QueueMutex;
        std::shared_mutex read_QueueMutex;
        std::mutex write_QueueMutex;
};

};