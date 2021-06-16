#include <string>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Refactor repeated 
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int hours = seconds/3600;
    int time = seconds%3600;
    int minutes = time/60;
    time = time % 60;
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