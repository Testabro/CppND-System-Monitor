#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

void Process::Pid(int pid) { pid_ = pid; }

float Process::CpuUtilization() const{
    float process_active_secs = LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK);
    float process_cpu_util = process_active_secs / LinuxParser::UpTime(pid_);

    return process_cpu_util;
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
    if (a.CpuUtilization() < CpuUtilization()) {
        return true;
    }
    return false;
}