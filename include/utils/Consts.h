#pragma once

#include "Types.h"
#include <string>

namespace utils
{
constexpr int BACKLOG{5};
constexpr int READSIZE{1024};
constexpr storage_size_t MAX_STORAGE_SIZE{60};
constexpr file_count_t MAX_FILE_COUNT{60};
const std::string PORT_NUM = "50020";
}
