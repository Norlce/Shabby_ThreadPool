#pragma once
#include"Job_queue.hpp"
#include<thread>

namespace ShabbyThreadPool{

class Excutor{
    private:
        Jobqueue* jobq;
    public:
        Excutor(Jobqueue* jq):jobq(jq){}
        void operator()(){
            if(jobq==nullptr) return;
            begin:
                if(!jobq->empty()){
                    auto job = jobq->job_pop();
                    job.do_();
                }
                else{
                    std::this_thread::yield();
                }
            goto begin;
        }
};

};