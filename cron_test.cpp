#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
using namespace std;

int main()
{
    // Create new file to test success of cron scheduler
    std::ofstream f("/home/ubuntu/AngryBirds/ok.txt");
    if(!f)
    {
        std::cerr<<"Cannot open the output file."<<std::endl;
        return 1;
    }
    f<<"\nSUCCESS\n";
    f.close();

    // NOTE: output is diverted to /var/log/syslog during cron
    // Loop to test kill script
    while (1)
    {
        cout << "SUCCESS" << endl;
        sleep(1);
    }
}
