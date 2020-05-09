// 2) Use the linus parser namespace to complete this class
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
  Processor system;
  this->cpu_ = system;
  return cpu_; 
}

// Create comparator function to sort the processes later
bool sortFn(Process const& a, Process const& b) { return a < b; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  
  vector<int> pid_list = LinuxParser::Pids();
  
  // For each pid, create a process object, then push object onto vector
  for (auto id : pid_list) {
    processes_.push_back(Process(id));
  }
  
  std::sort(processes_.begin(), processes_.end(), sortFn);
  
  return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {  
  return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}