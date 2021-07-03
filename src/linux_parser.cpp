#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  long memtotal = 0;
  long memfree = 0;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
           memtotal = std::stoi(value);
        }
        if (key == "MemFree:") {
           memfree = std::stoi(value);
        }
      }
    }
  }
  return (float)memfree / (float)memtotal;
}

long LinuxParser::UpTime() {
  string uptime_token;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_token;
  }
  long seconds_uptime = std::stoi(uptime_token);
  return seconds_uptime;
}

long LinuxParser::Jiffies() {
  return UpTime() * sysconf(_SC_CLK_TCK);
}

long LinuxParser::ActiveJiffies(int pid) {
  // Array values correspond to values as:
  // 13: Amount of time that this process has been scheduled in user mode, measured in clock ticks (divide by sysconf(_SC_CLK_TCK)).
  // 14: Amount of time that this process has been scheduled in kernel mode, measured in clock ticks (divide by sysconf(_SC_CLK_TCK)).
  string line;

  vector<string> jiffies;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string token;
    while ( linestream >> token ){
        jiffies.push_back(token);
    }
  }
  return std::stol(jiffies.at(13)) + std::stol(jiffies.at(14)); 
}

long LinuxParser::ActiveJiffies() { return std::stol(CpuUtilization().at(1)) + std::stol(CpuUtilization().at(3)); }

long LinuxParser::IdleJiffies() { return std::stol(CpuUtilization().at(4)); }

vector<string> LinuxParser::CpuUtilization() {
  // Array values correspond to values as:
  // 0: cpu
  // 1: user: normal processes executing in user mode
  // 2: nice: niced processes executing in user mode
  // 3: system: processes executing in kernel mode
  // 4: idle: twiddling thumbs
  // 5: iowait: In a word, iowait stands for waiting for I/O to complete. But there are several problems:
  // 6: irq: servicing interrupts
  // 7: softirq: servicing softirqs
  // 8: steal: involuntary wait
  // 9: guest: running a normal guest
  // 10:guest_nice: running a niced guest
  string line;

  vector<string> jiffies;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string token;
    while ( linestream >> token ){
        jiffies.push_back(token);
    }
  }
  return jiffies; 
}

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  string line;
  vector<string> command_elements;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string token;
    while ( linestream >> token ){
        command_elements.push_back(token);
    }
  }
  return command_elements.back();
}

string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          //Convert value from Kb to Mb before returning
          return std::to_string((std::stoi(value) / 1024));
        }
      }
    }
  } 
  return 0;
}

string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return 0;
}

string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line;
  string token;
  string user_name;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string intial_line = linestream.str();
      while (std::getline(linestream, token, ':')) {
        if (token == uid)
        { 
          return intial_line.substr(0, intial_line.find(":"));
        }
      }  
    }
  }  
  return "nouser";
}

long LinuxParser::UpTime(int pid) {
  //
  //(22) The time the process started after system boot. In kernels before Linux 2.6, this value was expressed in jiffies. 
  //Since Linux 2.6, the value is expressed in clock ticks (divide by sysconf(_SC_CLK_TCK)).
  //

  string clock_ticks;
  string line;
  int clock_ticks_index = 22;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < clock_ticks_index; i++) {
        linestream >> clock_ticks;
    }
  }
  return LinuxParser::UpTime() - (std::stoi(clock_ticks) / sysconf(_SC_CLK_TCK));
}
