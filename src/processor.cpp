#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  long active = LinuxParser::ActiveJiffies();
  long total = LinuxParser::Jiffies();
  // Use Active Jiffies / Total Jiffies
  float util = (float)(active) / (float)(total);
  
  return util; 

}