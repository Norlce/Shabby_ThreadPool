# Shabby_ThreadPool
基于c++标准库&lt;thread>的简易(陋)线程池

开箱即用的简陋线程池
正如其名字Shabby_ThreadPool(无论从音意还是本意都很符合)
采用单任务队列，无生产者线程，提供添加任务的接口

需要包含头文件ShabbyThreadPool.hpp，包含在名称空间ShabbyThreaadPool中定义

创建线程池(初始化):
  创建一个 ShabbyThreadPool::ThreadPool 线程池对象，构造函数接收一个整数，标识线程池内的线程个数
  exp:
    ShabbyThreadPool::ThreadPool tp(32);

添加任务:
  void
  ShabbyThreadPool::ThreadPool::job_add( /*Job, args....*/ );
    支持直接传递ShabbyThreadPool::Job或使用lambda等一切支持 "()" 运算符、接收任意多参数、返回值为void类型的对象;
    exp:
      tp.job_add([]{std::cout<<"Any operation"};);
      tp.job_add(
        [](int num){std::cout<<num;}
        ,3
      );
      tp.job_add( 
        ShabbyThreadPool::Job(
          []{std::cout<<"Any Job";}
        ) 
      );
      ShabbyThreadPool::Job job(
        [](int a, int b){
          std::cout<<a+b;
        }
        ,10086 ,114514
      );
      tp.job_add(job);

线程池开始运行:
  void 
  ShabbyThreadPool::ThreadPool::detach_run(/*void*/);

获取任务队列中任务剩余数量(非实时)：
  int 
  ShabbyThreadPool::ThreadPool::job_num(/*void*/);
  
Other:
  ShabbyThreadPool::Job
    任务的基本单位，有如下构造定义
    template<typename Func, typename ...Args>
    Job(Func fun, Args... arg) :func(fun), args({ arg... }) {}
    
TODO:
  异常处理
  线程池停止
  
Wating for next Version;
