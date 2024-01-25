#include "CollectorThread.h"

namespace collector
{
CollectorThread::CollectorThread(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv,
                                 bool& f, std::exception_ptr& err):
    traffic_storage{ts}, storage_mtx{m}, ready_to_write{cv}, finished{f}, error{err}
{

}

void CollectorThread::operator()(ThreadArg threadArg) try
{
    run(threadArg);
}
catch(const std::exception&)
{
    error = std::current_exception();
    finished = true;
}
}
