#include<list>
#include<tuple>
#include<iostream>

namespace ShabbyThreadPool{

template<typename Func, typename... Args>
class Job {
private:
    Func func;
    std::tuple<Args...> args;
    static const int arg_nums = sizeof...(Args);
    template<size_t ...N>
    void indirect_call(std::index_sequence<N...>) {
        func(std::get<N>(args)...);
    }
public:
    Job(Func fun, Args... arg) :func(fun), args({ arg... }) {};
    void do_() {
        indirect_call(std::make_index_sequence<arg_nums>{});
    }
};

class JobBase {
public:
    JobBase() {}
    virtual ~JobBase() {}
    virtual void do_() = 0;
    virtual JobBase* copy() = 0;
};

template<typename JobType>
class JobDerived :public JobBase {
private:
    JobType jobtype;
public:
    JobDerived(JobType jobt) :jobtype(jobt) {}
    virtual void do_() {
        jobtype.do_();
    }
    virtual ~JobDerived() {

    }
    virtual JobBase* copy() override{
        return new JobDerived<JobType>{ *this };
    }
};

class JobNode {
private:
    JobBase* jobbase;
public:
    template<typename T>
    JobNode(T t) :jobbase(new JobDerived(t)) {}
    JobNode(const JobNode& j) noexcept {
        this->jobbase = j.jobbase->copy();
    }
    JobNode(JobNode&& j) noexcept {
        this->jobbase = j.jobbase;
        j.jobbase = nullptr;
    }
    void do_() {
        jobbase->do_();
    }
    ~JobNode() {
        delete jobbase;
    }
};

};