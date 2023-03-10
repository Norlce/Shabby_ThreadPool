#include"Job.hpp"
#include<queue>
#include<mutex>

namespace ShabbyThreadPool{

class JobList{
    public:
        template<typename T = JobNodeBase>
        void job_add(T && t){
            std::lock_guard<std::mutex>queuelock(this->QueueMutex);
            this->JobQueue.push(std::forward<T>(t));
        }
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