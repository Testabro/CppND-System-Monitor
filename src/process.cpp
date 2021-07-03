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

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

void Process::Pid(int pid) { pid_ = pid; }

float Process::CpuUtilization() const {return (float)LinuxParser::ActiveJiffies(pid_) / (float)LinuxParser::ActiveJiffies(); }

// TODO: Return the command that generated this process
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