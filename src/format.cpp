#include <string>

#include "format.h"
#include <math.h> 

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  
  int hours = floor( seconds / 3600);
  int minutes = floor( seconds / 60);
  int seconds_left = seconds % 60;
  
  return to_string(hours) + to_string(minutes) + to_string(seconds_left); 
}