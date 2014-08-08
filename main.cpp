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
#include <deque>
#include <string>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "BlackLib.h"
#include "SensorSignal.h"


/* PREPROCESSOR CONSTANTS
 */
#define PRELIM	 false	// Record video only from bird collisions
#define DEBUG	 true 
#define PRETIME	 40	// 
#define POSTTIME 40	// 4 additional seconds after detection
#define FPS 	 20
#define X_RESOLUTION  320 	 
#define Y_RESOLUTION  240
#define WINDOW_SIZE 0        //!! DUSTIN
#define GROUND_THRESHOLD 0   //!! DUSTIN


using namespace cv;
using namespace std;


/* HELPER FUNCTION PROTOTYPES
 */
void create_directory(const char *path, struct stat &st);
string create_id(const char *path);


/*
 */
int main()
{

    Mat frame;
    queue <Mat> frames;
    deque <int> sensor_signal;   
    deque <int> averaged_signal; //!! DUSTIN 

    struct stat st;               
    string vid_id;               

    int limit = PRETIME;  
    int save = 0;

    float average_signal;
    float normal_signal;
	
    BlackADC* test_adc = new BlackADC(AIN4);
    const char * path = "/home/ubuntu/AngryBirds/SDCard/videos/";


//    if(PRELIM)
//    {
        queue <Mat> NCBuff; 
	int count = 0;
	const int NClimit = 600000;  //NClimit -> frames before auto save
//    }


/*---------------------------------------------------------------------*/

    VideoCapture input_cap(0);
 
    input_cap.set(CV_CAP_PROP_FRAME_WIDTH, X_RESOLUTION);
    input_cap.set(CV_CAP_PROP_FRAME_HEIGHT, Y_RESOLUTION);

    if (!input_cap.isOpened())
    {
        cout << "!!! Input video could not be opened" << endl;
        return -1;
    }

    while(input_cap.read(frame))
    {
	if(DEBUG)
        { cout << "number of frames saved = " << frames.size() << endl; }

        if(frames.size() >= limit)
        {
	    if(PRELIM)
	    {
                // Save every fourth frame
		if(count % 4 == 0)
		{
		    NCBuff.push(frames.front());
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
                create_directory(path, st);
                vid_id = create_id(path);

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

//-------------------- COLLISION DETECTION ----------------------//

/*
        SensorSignal * s = new typename SensorSignal::SensorSignal();
        s->SensorSignal::build_signal_deque(sensor_signal, SAMPLE_SIZE);
        normal_signal = s->SensorSignal::compute_normal_signal(sensor_signal, averaged_signal, WINDOW_SIZE);
        //!! Not needed at the moment !!//
        average_signal = s->SensorSignal::compute_average_signal(sensor_signal, SAMPLE_SIZE);
 
	//!! Get input from sensor_signal !!// 
        if (normal_signal > GROUND_THRESHOLD)
*/

        if (DEBUG) {
            cout << "ADC Analog Value: " << test_adc->getNumericValue() << endl;
        }
 
        if (test_adc->getNumericValue() > 60)
        {
            cout << "Event detected!" << endl;
            save = 1;
	    limit = POSTTIME;
	    if(DEBUG)
	    {
                cout << "ADC value exceeded 120!" << endl;
		cout << "Queue size extended" << endl; 
	    }
        }
	
        // Collision detected, save queue to write to file 
        if((save == 1) && (frames.size() >= limit))
        {  
            create_directory(path, st);
            if(PRELIM)
	    {
                vid_id = create_id(path) + "NO_COLLISION";
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
                // Push the frames from the buffer to the save file
		else	
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
            
            vid_id = create_id(path);
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
            {
                cout << "Number of frames saved: " << frames.size() << endl; 
            }

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
    cout << "!!! Video frame could not be read" << endl;
    input_cap.release();
}



/* FUNCTION DEFINITIONS
 */

/* Creates a new directory to store footage if it doesn't
 * exist
 */
void create_directory(const char *path, struct stat &st) 
{
    if(stat(path, &st) != 0) 
    {
        if(errno == ENOENT) 
        {
            cout << "Creating a new video directory" << endl;
            if(mkdir(path, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) == 0)
            { 
                perror("mkdir"); 
                }
            }
        }
}

/* Creates a new ID to name output video file
 * in format "Year-Month-Day Hour_Minute_Second"
 */
string create_id(const char *path) 
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    string vid_id;

    time(&rawtime);  
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%F %H_%M_%S", timeinfo);
    string new_id = string(buffer);
    vid_id = path + new_id + ".avi";

    return vid_id;
}

//-----EOF-----
