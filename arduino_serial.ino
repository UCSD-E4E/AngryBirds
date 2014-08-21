#include <SoftwareSerial.h>
#include <Wire.h>

#define rxPin 2   // RX pin for serial
#define txPin 3   // tx pin for serial
#define sclPin A5 // SCL pin for RTC
#define sdaPin A4 // SDA pin for RTC
 
// Software serial : RX = digital pin 2, TX = digital pin 3
SoftwareSerial serial(rxPin,txPin);

// Pin 12 will transmit the signal to BBB to sleep the video 
// capturing script once the IR signal has been recieved
int serial_out_pin = 12; // Pin 12 ??

void setup() {
    // Initialize pin 12 as output
    pinMode(serial_out_pin, OUTPUT);

    // Set up the serial monitor baud rate
    Serial.begin(115200);                 
    // Software serial port initialization - set the baud rate 
    // of the mini's serial port
    serial.begin(115200);       
    serial.listen();                        
}

void loop() {
    // Try to establish a connection with BBB
    if (serial.available() > 0) { 
        Serial.println("Connection available!");
        // If we recieve a signal from the IR remote, send a 
        // sleep flag to the BBB to stop the script
        if () { // What is the detection condition ?
            digitalWrite(serial_out_pin, HIGH);
        }
    } else {    
        Serial.println("No connection");
    }
}
