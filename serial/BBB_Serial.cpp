/*
 *   Defines functions in order for the Beaglebone Black to receive the date and time from the
 *   Arduino Pro Mini through a serial connection.
 *
 *   Written by Luke DeLuccia
 *   Date: 7/31/14
 */

#include <stdio.h>
#include <string>
#include "serialib.h"

#define         DEVICE_PORT             "/dev/ttyO2"                     // ttyO2 for linux = UART2 for Beaglebone Black
#define BAUD_RATE 115200

serialib LS;                                                            // Object of the serialib class
int Ret;                                                                // Used for return values
char Buffer[128];

/*
 *   Opens the serial port between the Beaglebone Black and Arduino Pro Mini, signals the Arduino to send the
 *   current date and time, and returns the date and time in char[] format
 */
std::string getDateAndTimeFromArduino(){
	
    // open the port first
    openSerialPort();                                                      // Open serial port at baud rate of 115200
    
    // tell the Arduino that we want the date and time
    Ret = LS.WriteString("@ \n");                                            // Send the command on the serial port
    if (Ret!=1) {                                                           // If the writting operation failed ...
        printf ("Error while writing data!\n");                              // ... display a message ...
        return Ret;                                                         // ... quit the application.
    }
    printf ("Write operation is successful!\n");
    
    // grab the date and time from the Arduino
    Ret=LS.ReadString(Buffer,'\n',128,5000);                                // Read a maximum of 128 characters with a timeout of 5 seconds
    // The final character of the string must be a line feed ('\n')
    if (Ret>0){                                                            // If a string has been read from, print the string
        printf ("String read from serial port : %s",Buffer);
        return Buffer;
    }
    else
        printf ("TimeOut reached. No data received!\n");                   // If not, print a message.
    
    // close the port to save power
    closeSerialPort();							    // Close the serial port
    
    return "";
    
}

/*
 *   Opens the serial port between the Beaglebone Black and the Arduino Pro Mini at the given BAUD_RATE
 */
void openSerialPort(){
    Ret=LS.Open(DEVICE_PORT,BAUD_RATE);                                        // Open serial link at 115200 bauds
    if (Ret!=1) {                                                           // If an error occured...
        printf ("Error while opening port. Permission problem ?\n");        // ... display a message ...
        return;                                                         // ... quit the application
    }
    printf ("Serial port opened successfully !\n");
}

/*
 *   Closes the serial port between the Beaglebone Black and the Arduino Pro Mini
 */
void closeSerialPort(){ LS.Close();
}


