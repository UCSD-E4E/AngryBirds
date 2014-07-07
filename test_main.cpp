/* Filename: test_main.cpp
 * Author(s):  
 * Date: 8/8/14
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


/*-------------------------------------------------
             PREPROCESSOR CONSTANTS
  -------------------------------------------------*/
#define PRELIM	         false	 
#define DEBUG	         true        
#define PRETIME	         50	   
#define POSTTIME         100    
#define FPS 	         20
#define X_RESOLUTION     800    	 
#define Y_RESOLUTION     600    
#define WINDOW_SIZE      0       
#define GROUND_THRESHOLD 0  
#define TEST_THRESHOLD   1000 

using namespace cv;
using namespace std;


/*-----------------------------------------------------
              FUNCTION PROTPOTYPES
  -----------------------------------------------------*/
void create_directory(const char *path, struct stat &st);
string create_id(const char *path, bool collision);


int main()
{
  /*---------------------------------------------------
          VARIABLE DECLARATIONS / INITIALIZATION
    ---------------------------------------------------*/
    Mat     frame;
    queue   <Mat> frames;
    deque   <int> sensor_signal;   
    deque   <int> averaged_signal;       // DUSTIN 
    struct  stat st;               
    string  vid_id;               
    float   average_signal  = 0;
    float   normal_signal   = 0;        
    int     frame_count     = 0;         // For Testing Purposes
    int     test_count      = 0;         // For Testing Purposes 
    int     max_count       = 50;        // For Testing Purposes
    int     limit           = PRETIME;  
    bool    save            = false;
    bool    detected        = false;
    bool    collision       = false;

    BlackADC* test_adc = new BlackADC(AIN4);
    const char * path = "/home/ubuntu/AngryBirds/SDCard/videos/";


  /*-----------------------------------------------
     FRAME CAPTURE / STORAGE + COLLISION DETECTION
    -----------------------------------------------*/

    /* Open video no.0 */
    VideoCapture input_cap(0);
 
    /* Set (lower) the resolution for the webcam 
     * (original res: 1280 x 720) */
    input_cap.set(CV_CAP_PROP_FRAME_WIDTH, X_RESOLUTION);
    input_cap.set(CV_CAP_PROP_FRAME_HEIGHT, Y_RESOLUTION);

    /* Open the camera for capturing, if failure, terminate */
    if (!input_cap.isOpened())
    {
        cout << "\nINPUT VIDEO COULD NOT BE OPENED\n" << endl;
        return -1;
    }
  
    /* Read in each frame for storage and processing */
    while(input_cap.read(frame))
    {
        if(frames.size() >= limit)
        {
	    frames.pop();
            frames.push(frame.clone());
        }
        else
        {
            frames.push(frame.clone());
        }
 
        /* Basic test condition (for testing purposes - to be 
         * changed). Flag if particular signal exceeds test threshold
         * otherwise, proceed with continuous footage capture */
        if ((test_adc->getNumericValue() > TEST_THRESHOLD) ||
            (test_count >= max_count))
        {
            if (test_adc->getNumericValue() > TEST_THRESHOLD) 
            {
                 detected = true;
            }
            save = true;
	    limit = POSTTIME;
        }

        if (DEBUG) 
        {
            cout << "TEST_COUNT: " << test_count << endl;        
            cout << "FRAME SIZE: " << frames.size() << endl;
            cout << "SAVE: " << save << endl;   
        }
	
        /* Event detected, save queue to write to output file */
        if((save) && (frames.size() >= limit))
        { 
            if (DEBUG)
            {
                if (detected)
                { 
                    cout << "\nEVENT TRIGGERED!\n" << endl;
                    collision = true;
                }
                else 
                {
                    cout << "\nSAVING SCHEDULED NON-COLLISON CLIP\n" << endl;
                }
            }

            if (DEBUG) {cout << "\nCREATING VIDEO\n" << endl;}

            /* Create the output destination */
            create_directory(path, st);
            vid_id = create_id(path, collision);
	    VideoWriter output_cap(vid_id, 
                                   CV_FOURCC('M','J','P','G'),
                                   FPS, 
                                   Size(X_RESOLUTION, Y_RESOLUTION), 
                                   true);

            if(!output_cap.isOpened())
            {
                cout << "\nOUTPUT VIDEO COULD NOT BE OPENED\n" << endl;
                return -1;
            }

            if (DEBUG) {cout << "\nPUSHING FRAMES\n" << endl;}

            /* Write collision seqeuence to output file */
            while(!frames.empty())
            {
                if (DEBUG) {cout << "WRITING FRAME: " << frame_count << endl;}
                output_cap.write(frames.front());
                frames.pop();
                frame_count += 1;
            }
            frame_count = 0;

            if (DEBUG) {cout << "\nDONE WRITING\n" << endl;}

            output_cap.release();
	    limit = PRETIME;
            save = 0;
            test_count = 0;
            collision = false;
        }
        test_count++;
    }
    /* Exit */
    input_cap.release();
}



/*-----------------------------------------------------
              FUNCTION DEFINITIONS
  -----------------------------------------------------*/
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
string create_id(const char *path, bool collision) 
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);  
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%F %H_%M_%S", timeinfo);
    string new_id = string(buffer);
    if (collision) 
    {
        return (path + new_id + ".avi");
    }
    else 
    {
        return( path + new_id + "_NC" + ".avi");
    }
}

//-----EOF-----
