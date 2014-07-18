//Author: Dylan McNamara
//Date Created: 07/18/2014
//Date Modified: 07/18/2014
//Description: Record Analog Input

#include "BlackLib.h"
#include <iostream>
#include <conio.h>
#define SAMPLE_RATE 1
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
    BlackADC Receiver;
    do
    {
	prompt();
	cin >> input;
    }while(!(6 >= input && input >= 0));
    switch(input)
    {
	case 0:
	    Receiver(AIN0);
	    break;
	case 1:
	    Receiver(AIN1);
	    break;
	case 2:
	    Receiver(AIN2);
	    break;
	case 3:
	    Receiver(AIN3);
	    break;
	case 4:
	    Receiver(AIN4);
	    break;
	case 5:
	    Receiver(AIN5);
	    break;
	case 6:
	    Receiver(AIN6);
	    break;
    }

    do
    {
	cout << Receiver.
    }while(!kbhit());
}
