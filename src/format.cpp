#include <string>
#include <iomanip>

#include "format.h"

using std::string;

const int SEC_PER_HR = 3600;
const int SEC_PER_MIN = 60;

string Format::ElapsedTime(long seconds) {
    int hours = seconds / SEC_PER_HR;
    int time = seconds % SEC_PER_HR;
    int minutes = time / SEC_PER_MIN;
    time = time % SEC_PER_MIN;
    int seconds_remaining = time;

    std::ostringstream stream_time;
    stream_time << std::setw(2) << std::setfill('0') << std::to_string(hours) << ":"
        << std::setw(2) << std::setfill('0') << std::to_string(minutes) << ":"
        << std::setw(2) << std::setfill('0') << std::to_string(seconds_remaining);

    return stream_time.str();
}