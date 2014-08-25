/* Code to include in main to read time from RTC on Beaglebone Black
 */

#include <string>
#include <iostream>
#include <stdio.h>
#include <typeinfo>

using namespace std;

std::string exec(char* cmd);


int main(){
    std::string time;
    std::string formatted_time;

    // Read the time from the RTC
    time = exec("hwclock -r -f /dev/rtc1"); 
    std::cout << "Time: " << time << std::endl;

    // Parse the output
    formatted_time.append(time, 4);
    formatted_time.append(time, 5);
    
    std::cout << formatted_time << std::endl;

    // Set the System Clock
}


std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}
