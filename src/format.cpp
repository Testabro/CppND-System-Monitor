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

    std::ostringstream ostr_hr;
    ostr_hr << std::setw(2) << std::setfill('0') << std::to_string(hours);
    string hours_formatted = ostr_hr.str();

    std::ostringstream ostr_min;
    ostr_min << std::setw(2) << std::setfill('0') << std::to_string(minutes);
    string minutes_formatted = ostr_min.str();

    std::ostringstream ostr_sec;
    ostr_sec << std::setw(2) << std::setfill('0') << std::to_string(seconds_remaining);
    string seconds_formatted = ostr_sec.str();

    string elasped_time = hours_formatted + ":" + minutes_formatted + ":" + seconds_formatted;
    return elasped_time;
}