#pragma once

#include "Types.h"
#include <string>

namespace utils
{
constexpr int BACKLOG{5};
constexpr int READSIZE{1024};
constexpr file_count_t MAX_FILE_COUNT{60};
const std::string PORT_NUM = "50020";
const storage_size_t storage_size = 1;
}
