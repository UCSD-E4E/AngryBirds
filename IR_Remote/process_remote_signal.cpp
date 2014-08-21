#include <time.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "../BlackLib/BlackLib.h"

using namespace std;

void flash_led(BlackGPIO *led_out, int num_times);



int main() {
   // Change/Check GPIO Pin #
   BlackGPIO *ir_in = new BlackGPIO(GPIO_68, input);       // P8_10
   //BlackGPIO *led_out = new BlackGPIO(GPIO_68, output);  // P8_10

   while (1) {
       if (ir_in->fail()) {
           cout << "ERROR" << endl;
       }
       cout << "GPIO pin value: " << ir_in->getValue() << endl;
       if (ir_in->isHigh()) {
           //flash_led(led_out, 5);
           cout << "SIGNAL HEARD!" << endl;
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

