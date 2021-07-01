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

float Process::CpuUtilization() const { return (float)LinuxParser::ActiveJiffies(pid_) / (float)LinuxParser::ActiveJiffies(); }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

bool Process::operator<(Process const& a) const {
    if (CpuUtilization() < a.CpuUtilization()) {
        return true;
    }
    return false;
}