// 1) Start with filling in the todo's here
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stol;
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
  string label1, label2, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> label1 >> label2 >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float total_mem, free_mem; //initialise both as floats since the output of the calculation will be float
  float mem_util;
  string line1, line2;
  string label1, label2;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    // Call getline to get 1st line - total memory
    std::getline(stream, line1);
    std::istringstream linestream1(line1);
    linestream1 >> label1 >> total_mem;
    
    // Call getline again - free memory
    std::getline(stream, line2);
    std::istringstream linestream2(line2);
    linestream2 >> label2 >> free_mem;
  }
  
  // Utilisation calculation
  mem_util = (total_mem - free_mem) / total_mem;
  
  return mem_util;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {   
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  
  if (stream.is_open()) {
    // Call getline to get 1st line and just parse 1st token
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  
  return uptime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total_jiffies=0;
  vector<string> util_string = LinuxParser::CpuUtilization();
  
  // loop through each and sum up all except for guest and guestnice, since they are already accounted for in usertime and nicetime (according to htop source code)
  for (unsigned int i = 0; i < util_string.size(); i++) {
    if(i != LinuxParser::CPUStates::kGuest_ && i != LinuxParser::CPUStates::kGuestNice_) {
      total_jiffies += stol(util_string[i]);
    }
  }
  
  return total_jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long total_jiffies = 0;
  string token;
  string line;
  int index = 1;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (stream.is_open()) {
    // Call getline multiple times to search until label = cpu
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
     
      while (linestream >> token) {
        if (index == 14 || index == 15 || index == 16 || index == 17) {
          total_jiffies += stol(token);
        }
        index++;
      }
    }
  }
  return total_jiffies;
}
// index 14-17 and 22

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long total_jiffies = LinuxParser::Jiffies();
  long idle_jiffies = LinuxParser::IdleJiffies();
  // total jiffies - idle jiffies
  long active_jiffies = total_jiffies - idle_jiffies;
  
  return active_jiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  vector<string> util_string = LinuxParser::CpuUtilization();
  
  // add only idletime and iowait
  long idle_jiffies = stol(util_string[LinuxParser::CPUStates::kIdle_]) + stol(util_string[LinuxParser::CPUStates::kIOwait_]);
  
  return idle_jiffies;
}

// TODO: Read and return CPU utilization - this is a vector of the strings in the stat text file
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> util_strings;
  string label;
  string line;
  string token;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
    // Call getline multiple times to search until label = cpu
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label;
      if (label == "cpu") {
        // Push the next token onto end of vector
        while (linestream >> token)
         util_strings.push_back(token);
      }
    }
  }
  
  return util_strings;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int total_procs = 0; // initialise to default value of 0
  string label;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
    // Call getline multiple times to search until label = processes
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label >> total_procs;
      if (label != "processes") {
        return total_procs;
      }
    }
  }
  
  // if function gets to here, just return default value
  return total_procs;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int procs_running = 0; // initialise to default of 0
  string label;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if (stream.is_open()) {
    // Call getline multiple times to search until label = processes
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label >> procs_running;
      if (label == "procs_running") {
         return procs_running;
      }
      
    }
  }
  return procs_running;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  
  if (stream.is_open()) {
    // Call getline to get full line
    std::getline(stream, line);
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string ram;
  string label;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  if (stream.is_open()) {
    // Call getline multiple times to search until label = cpu
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label;
      if (label == "VmSize:") {
        linestream >> ram;
    }
  }
  }
  
  return to_string(stol(ram)/1024) + " MB";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string uid;
  string label;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  if (stream.is_open()) {
    // Call getline multiple times to search until label = cpu
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label;
      if (label == "Uid:") {
        linestream >> uid;
    }
  }
  }
  
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid;
  string label;
  string user = "unknown";
  string line;
  std::ifstream stream(kPasswordPath);
  
  if (stream.is_open()) {
    // Call getline multiple times to search until label = cpu
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> label >> uid;
      if (uid == LinuxParser::Uid(pid)) {
        return user;
      }
    }
  }
  //if function gets to here, just return the default value it was initialised to
  return user; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long starttime;
  string token;
  string line;
  
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (stream.is_open()) {
    // Call getline multiple times to search until label = cpu
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      int index=1;
      
      while (linestream >> token) {
        if(index == 22) {
          starttime = stol(token);
          break;
        }
        index++;
      }
       
    }
  }
  
  return starttime / sysconf(_SC_CLK_TCK);
}