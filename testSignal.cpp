//Author: Dylan McNamara
//Date Created: 07/18/2014
//Date Modified: 07/18/2014
//Description: Record Analog Input

#include "BlackLib.h"
#include <iostream>
#include <time.h>

#define SAMPLE_TIME 0 //In milliseconds


using namespace std;

//Method sleep copied from: http://ubuntuforums.org/showthread.php?t=296142
void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

void prompt()
{
    cout << "Which Analog port would you like to receive from:" << endl;
    cout << "\t0: pin 39" << endl;
    cout << "\t1: pin 40" << endl;
    cout << "\t2: pin 37" << endl;
    cout << "\t3: pin 38" << endl;
    cout << "\t4: pin 33" << endl;
    cout << "\t5: pin 36" << endl;
    cout << "\t6: pin 35" << endl;
}

int main()
{
    int input;
    BlackADC* Receiver;
    do
    {
	prompt();
	cin >> input;
    } while(!(6 >= input && input >= 0));
    
    switch(input)
    {
	case 0:
	    Receiver = new BlackADC(AIN0);
	    break;
	case 1:
	    Receiver = new BlackADC(AIN1);
	    break;
	case 2:
	    Receiver = new BlackADC(AIN2);
	    break;
	case 3:
	    Receiver = new BlackADC(AIN3);
	    break;
	case 4:
	    Receiver = new BlackADC(AIN4);
	    break;
	case 5:
	    Receiver = new BlackADC(AIN5);
	    break;
	case 6:
	    Receiver = new BlackADC(AIN6);
	    break;
    }

    do
    {
	sleep(SAMPLE_TIME);
	cout << Receiver->getNumericValue() << endl;
    } while(1);
}
