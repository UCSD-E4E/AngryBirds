// test
#include "opencv2/core/core.hpp"
#include <time.h>
#include "opencv2/highgui/highgui.hpp"
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
#define DEBUG false
#define PRETIME 20	// 10 FPS, 2 SEC
#define POSTTIME 40	// 2 additional seconds after detection

using namespace cv;
using namespace std;

int main()
{
    VideoCapture input_cap(0);
    Mat frame;
    queue <Mat> frames;
    string vid_id;
    int limit = PRETIME; 
    int save = 0;
    time_t rawtime;
    struct tm * timeinfo;
    struct stat st;
    const char* path;
    char buffer[80];

    //ADC Ground = pin 34
    //AIN0 = pin 39
    //AIN1 = pin 40
    BlackADC* test = new BlackADC(AIN0);

    if (!input_cap.isOpened())
    {
        cout << "!!! Input video could not be opened" << endl;
        return -1;
    }

    while(input_cap.read(frame))
    {
        
        if(frames.size() >= limit)
        {
	    if(DEBUG)
            {printf("number of frames saved = %d\r\n", int(frames.size()));}
            frames.pop();
            frames.push(frame.clone());
        }
        else
        {
            if(DEBUG)
	    {printf("number of frames saved = %d\r\n", int(frames.size()));}
            frames.push(frame.clone());
        }


//!!	-------------------------------------------
        if(test->getNumericValue() > 1000 /*i == 101*/) // Detect collision --> triggers event 
        {
            cout << "Event detected!" << endl;
            save = 1;
//!!	    cout << "Queue size extended" << endl;
            limit = POSTTIME;
	    cout << frames.size() << endl;
        }
//!!	-------------------------------------------
	cout << frames.size() << endl;

        // Collision detected, save queue to write to file 
        if((save == 1) && (frames.size() >= limit))
        {   
            // Create new directory to store footage if DNE 
            //path = "/home/e4e/opencv/samples/videos/";
            path = "/home/ubuntu/SDCard/videos/";
            if (stat(path, &st) != 0) 
            {
                if (errno == ENOENT) 
                {
                    cout << "Creating a new video directory" << endl;
                    if (mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
                    {
                       perror("mkdir");
                    }
                }
            }
            
            // Create unique identifier for video file
            time(&rawtime);  
            timeinfo = localtime(&rawtime);
            strftime(buffer, 80, "%F %T", timeinfo);
	    string new_id = string(buffer);
            vid_id = path + new_id + ".avi";

	    VideoWriter output_cap(vid_id, 
                                   CV_FOURCC('M','J','P','G'), 
                                   10.0, 
                                   Size(input_cap.get(CV_CAP_PROP_FRAME_WIDTH), 
                                   input_cap.get(CV_CAP_PROP_FRAME_HEIGHT)), 
                                   true);

            if (!output_cap.isOpened())
            {
                cout << "!!! Output video could not be opened" << endl;
                return -1;
            }

//!!            printf("number of frames saved = %d\r\n", int(frames.size()));

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

