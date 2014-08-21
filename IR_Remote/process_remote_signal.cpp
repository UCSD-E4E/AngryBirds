#include <time.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "../BlackLib/BlackLib.h"

using namespace std;

void flash_led(BlackGPIO *led_out, int num_times);



int main() {
   // Change/Check GPIO Pin #
   BlackGPIO *serial_in = new BlackGPIO(GPIO_3, input);  // P9_18
   BlackGPIO *led_out = new BlackGPIO(GPIO_68, output);  // P8_10
   string serial_msg;

   while (1) {
       // -------------  DETECT IR REMOTE SIGNAL --------- //
       serial_msg = serial_in->getValue();
       if (serial_in->fail()) {
           cout << "ERROR" << endl;
       }
       cout << "GPIO pin value: (SERIAL)" << serial_msg << endl;
       // If msg matches *something* we've recieved the GO signal 
       // Turn on LED light to indicate success
       if (serial_in->isHigh()) {
           flash_led(led_out, 5);
           // DO SOMETHING HERE
       }
    } 
}




/* 
 */
void flash_led(BlackGPIO *led_out, int num_times) {
    cout << "IN FLASH LED" << endl;
    if (led_out->fail()) {
        cout << "ERROR" << endl;
    }

    int i = 0;
    while (i < num_times) {
        cout << "i: " << i << endl;
        cout << "LED_OUT VALUE_start: " << led_out->getValue() << endl;
        led_out->setValue(high);
        cout << "LED_OUT VALUE_set high: " << led_out->getValue() << endl;
        sleep(1);
        led_out->setValue(low);
        cout << "LED_OUT VALUE_set low: " << led_out->getValue() << endl;
        sleep(1);
        i++;
    }
}

