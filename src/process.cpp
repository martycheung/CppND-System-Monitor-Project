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
int Process::Pid() { 
  
  return this->pid; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  
  long proc_jiffies = LinuxParser::ActiveJiffies(this->pid);
  long total_jiffies = LinuxParser::Jiffies();
  
  float proc_util = (float)proc_jiffies / (float)total_jiffies;
  
  this->cpuUtilization_ = proc_util;
  
  return proc_util; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
  
  string command = LinuxParser::Command(this->pid);
  
  return command; 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
  
  string ram = LinuxParser::Ram(this->pid);
  
  return ram; 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  
  string user = LinuxParser::User(this->pid);
  
  return user; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  
  long int uptime = LinuxParser::UpTime(this->pid);
  
  return uptime; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {

  return a.cpuUtilization_ < this->cpuUtilization_;

}