/* Filename: main.cpp
 * Author(s): Alireza Khodamoradi, Angela To, Dylan McNamara
 * Date Created: 06/24/2014 
 * Date Modified: 07/18/2014
 * Description: Detects when a bird hits a mirror and records the collision:
 *	-Designed to work with the BeagleBone Black
 *	-Uses OpenCV Libraries to read input from a camera
 *	-Uses BlackLib Library to read analog input from BeagleBone Black
 */

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <queue>
#include <string>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "BlackLib.h"

//-----PREPROCESSOR CONSTANTS-----
#define PRELIM	 true	// --false: Record video only from bird collisions
#define DEBUG	 true 
#define PRETIME	 20	// 10 FPS, 2 SEC
#define POSTTIME 40	// 4 additional seconds after detection
#define FPS 	 20
#define X_RESOLUTION  320 //352 //320	 
#define Y_RESOLUTION  240 //288 //240
#define CLEAR 0
#define PEAK 1
#define TROUGH 2
#define HILL 3

using namespace cv;
using namespace std;


enum stages {CLEAR, PEAK, TROUGH, HILL}


int main()
{
    // OpenCV Variables (used to capture video)
    VideoCapture input_cap(0);
 
    // Set the resolution of the camera
    input_cap.set(CV_CAP_PROP_FRAME_WIDTH, X_RESOLUTION);
    input_cap.set(CV_CAP_PROP_FRAME_HEIGHT, Y_RESOLUTION);

    Mat frame;
    queue <Mat> frames;

    //if(PRELIM)
    //{ 
	queue <Mat> NCBuff; 
    //}

    // Variables needed to create filename
    time_t rawtime;
    struct tm * timeinfo;
    struct stat st;
    char buffer[80];
    string vid_id;
    const char* path;
    
    // Variables needed for logic of code
    int limit = PRETIME; 
    int save = 0;
    stages stage = CLEAR

    //if(PRELIM)
    //{ 
	int count = 0;
	const int NClimit = 6000;  //NClimit -> frames before auto save
    //}

    // ADC Ground = pin 34
    // AIN0 = pin 39
    // AIN1 = pin 40
    BlackADC* test = new BlackADC(AIN0);

    path = "/home/ubuntu/SDCard/videos/";
    
    // Check if the video feed was opened correctly
    if (!input_cap.isOpened())
    {
        cout << "!!! Input video could not be opened" << endl;
        return -1;
    }

    while(input_cap.read(frame))
    {
        // Limit the size of the buffer
	if(DEBUG)
        { cout << "number of frames saved = " << frames.size() << endl; }

        if(frames.size() >= limit)
        {
	                
	    if(PRELIM)
	    {
		if(count % 4 == 0)
		{
		    NCBuff.push(frames.front());// Save 1 of every 4 frames
		}
		count++;
	    }
	    frames.pop();
            frames.push(frame.clone());
        }
        else
        {
            frames.push(frame.clone());
        }
	
	// If too much time has passed: 
	//	-Save video in the buffer
	//	-Reset count and buffer
	if(PRELIM)
	{
	    if (count == NClimit)
	    {
		//Save the current video
		// Create new directory to store footage if it doesn't exist 
                if(stat(path, &st) != 0) 
            	{
                    if(errno == ENOENT) 
                    {
                    	cout << "Creating a new video directory" << endl;
                    	if(mkdir(path, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) == 0)
                    	{ perror("mkdir"); }
                    }
            	}
            	// Create saved for noncollision video
		time(&rawtime);  
        	timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%F %H_%M_%S", timeinfo);
		string new_id = string(buffer) + "NO_COLLISION";
		vid_id = path + new_id + ".avi";

	   	VideoWriter output_cap(vid_id, 
                                   CV_FOURCC('M','J','P','G'),
                                   FPS, 
                                   Size(X_RESOLUTION, Y_RESOLUTION), 
                                   true);
		// Check and exit if there is an error saving the video
           	if(!output_cap.isOpened())
            	{
                    cout << "!!! Output video for NCBuff could not be"
		    	 <<" opened" << endl;
            	}
		else    // Push the frames from the buffer to the save file
		{
           	    while(!NCBuff.empty())
            	    {
                        output_cap.write(NCBuff.front());
                        NCBuff.pop();
	  	        cout << "Non-Collision Video Saved" << endl;
		    }
		}
		output_cap.release();
		count = 0;
	    }
	}
	// Check to see if analog input is greater than 1V (detects collision)
	// If true, extend the buffer and flag for a save operation
        if (test->getNumericValue() > 1000)
        {
            cout << "Event detected!" << endl;
            save = 1;
	    limit = POSTTIME;
	    if(DEBUG)
	    {
		cout << "Queue size extended" << endl; 
	    }
        }
	
        // Collision detected, save queue to write to file 
        if((save == 1) && (frames.size() >= limit))
        {   
            // Create new directory to store footage if it doesn't exist 
            if(stat(path, &st) != 0) 
            {
                if(errno == ENOENT) 
                {
                    cout << "Creating a new video directory" << endl;
                    if(mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
                    {
                       perror("mkdir");
                    }
                }
            }
            if(PRELIM)
	    {
            	// Create saved for noncollision video
		time(&rawtime);  
        	timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%F %H_%M_%S", timeinfo);
		string new_id = string(buffer) + "NO_COLLISION";
		vid_id = path + new_id + ".avi";

	   	VideoWriter output_cap(vid_id, 
                                   CV_FOURCC('M','J','P','G'),
                                   FPS, 
                                   Size(X_RESOLUTION, Y_RESOLUTION), 
                                   true);
		// Check and exit if there is an error saving the video
           	if(!output_cap.isOpened())
            	{
                    cout << "!!! Output video for NCBuff could not be opened"
			<< endl;
            	}
		else	// Push the frames from the buffer to the save file
		{
           	    while(!NCBuff.empty())
            	    {
                        output_cap.write(NCBuff.front());
                        NCBuff.pop();
		        cout << "Non-Collision Video Saved" << endl;
		    }
		}
		output_cap.release();
		count = 0;
	    }

            // Create name for saved video: "Year-Month-Day Hour_Minute_Second"
            time(&rawtime);  
            timeinfo = localtime(&rawtime);
            strftime(buffer, 80, "%F %H_%M_%S", timeinfo);
	    string new_id = string(buffer);
            vid_id = path + new_id + ".avi";

	    VideoWriter output_cap(vid_id, 
                                   CV_FOURCC('M','J','P','G'),
                                   FPS, 
                                   Size(X_RESOLUTION, Y_RESOLUTION), 
                                   true);
	    // Check and exit if there is an error saving the video
            if(!output_cap.isOpened())
            {
                cout << "!!! Output video could not be opened" << endl;
                return -1;
            }

	    if(DEBUG)
            { printf("number of frames saved = %d\r\n", int(frames.size())); }
	    
	    // Push the frames from the buffer to the save file
            while(!frames.empty())
            {
                output_cap.write(frames.front());
                frames.pop();
		cout << "Video Saved";
            }
            output_cap.release();
            save = 0;
	    limit = PRETIME;
        }
    }

    // Only exit the while loop if there is an error reading a frame
    // Otherwise, exit code by using a keyboard interrup (Ctrl-C)
    cout << "!!! Video frame could not be read" << endl;
    input_cap.release();
}
//-----EOF-----
