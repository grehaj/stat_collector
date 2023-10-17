#include "TrafficWritter.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

namespace collector
{
namespace fs = std::filesystem;

std::string get_file_name(int num, const std::string& directory)
{
    char buffer[100]{};
    if(not directory.empty())
    {
        fs::path target_path = fs::path{directory} / utils::LOG_TEMPLATE;
        sprintf(buffer, target_path.c_str(), num);
    }
    else
        sprintf(buffer, utils::DEFAULT_LOG_LOCATION.c_str(), num);

    return std::string{buffer};
}

TrafficWritter::TrafficWritter(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv,
                               bool& finished, std::exception_ptr& error):
    CollectorThread{ts, m, cv, finished, error}
{
}

void TrafficWritter::run(ThreadArg threadArg)
{
    file_count_t file_num = 1;
    fs::path file_path = fs::path{get_file_name(file_num, threadArg.directory)};

    while(true)
    {
        std::unique_lock<std::mutex> ul{storage_mtx};
        ready_to_write.wait(ul, [&]{return traffic_storage.size() == threadArg.storage_size;});
        std::ofstream out{file_path.string()};
        out << traffic_storage;
        traffic_storage.clear();
        ++file_num;
        if(file_num > threadArg.file_count)
        {
            finished = true;
            return;
        }
        file_path = fs::path{get_file_name(file_num, threadArg.directory)};
    }
}
}
