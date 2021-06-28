#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {

    long active_jiffs = LinuxParser::ActiveJiffies();
    long idle_jiffs = LinuxParser::IdleJiffies();
    
    long delta_active_jiffs = std::abs(active_jiffs - _prev_active_jiffs);
    long delta_idle_jiffs = std::abs(idle_jiffs - _prev_idle_jiffs);

    _prev_active_jiffs = LinuxParser::ActiveJiffies();
    _prev_idle_jiffs = LinuxParser::IdleJiffies();

    return (float)delta_active_jiffs / (float)delta_idle_jiffs;
}