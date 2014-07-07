#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "BlackLib.h"

using namespace std;

int main() 
{
 				   // Ground = 34
    BlackADC *adc = new BlackADC(AIN4); // Pin 33 
    float analog; 
    float converted;

    while (1) 
    {
        if (adc->fail()) 
        {
            cerr << "Error" << endl;
        }

        analog = adc->getNumericValue();
        converted = adc->getParsedValue(dap3);      

        cout << "analog_input: " << analog << endl;
        cout << "converted voltage: " << converted << endl; 
        cout << "\n" << endl;

        if (analog > 60 ){
            sleep(3);
        }

        //sleep(0.5);
    }
}



