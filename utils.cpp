#include "utils.h"

#include <ctime>
#include <chrono>

timeLong Utils::getMillisecOfNow()
{
    const auto now = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}
