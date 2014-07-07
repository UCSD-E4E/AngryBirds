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
#include <thread>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include "BlackLib.h"
#include "SensorSignal.h"

using namespace cv;
using namespace std;

/*-------------------------------------------------
             PREPROCESSOR CONSTANTS
  -------------------------------------------------*/
#define DEBUG	         true
#define PRETIME	         50    // 100
#define POSTTIME         100    // 1500
#define FPS 	         20
#define X_RESOLUTION     352
#define Y_RESOLUTION     288
#define WINDOW_SIZE      0
#define GROUND_THRESHOLD 0
#define TEST_THRESHOLD   700



/*-----------------------------------------------------
              FUNCTION PROTPOTYPES
  -----------------------------------------------------*/
void create_directory(const char *path, struct stat &st);
string create_id(const char *path, bool collision);
//void update_frames(queue<Mat> &frames, Mat &frame, int limit);
void *update_frames(void* update_args);
string get_date();

// Struct containing necessary values for update_frames function
struct update_struct{
    queue<Mat> frames;
    Mat frame;
    int limit;
};

int main(){
  /*---------------------------------------------------
          VARIABLE DECLARATIONS / INITIALIZATION
    ---------------------------------------------------*/
    Mat         frame;
    queue       <Mat> frames;
    deque       <int> sensor_signal;
    deque       <int> averaged_signal;       // DUSTIN
    struct      stat st;
    struct      update_struct update_args;
    string      vid_id;
    pthread_t   update_thread;
    float       average_signal  = 0;
    float       normal_signal   = 0;
    int         thread_ret      = 0;
    int         frame_count     = 0;         // For Testing Purposes
    int         test_count      = 0;         // For Testing Purposes
    int         max_count       = 50;        // For Testing Purposes
    int         limit           = PRETIME;
    bool        save            = false;
    bool        detected        = false;
    bool        collision       = false;
    //Get the input from adc
    BlackADC *test_adc = new BlackADC(AIN4);
    //Path to save video files
    const char *path = "/home/ubuntu/AngryBirds/SDCard/videos/";
    ofstream signals;

  /*---------------------------------------------------
       FRAME CAPTURE/STORAGE + COLLISION DETECTION
    ---------------------------------------------------*/
    // Open video capture device #0
    VideoCapture input_cap(0);

    // Set (lower) the resolution for the webcam
    //since we don't need 1080p of nothing
    input_cap.set(CV_CAP_PROP_FRAME_WIDTH, X_RESOLUTION);
    input_cap.set(CV_CAP_PROP_FRAME_HEIGHT, Y_RESOLUTION);

    // Open the camera for capturing, if failure, terminate
    if (!input_cap.isOpened()){
        cout << "\nINPUT VIDEO COULD NOT BE OPENED\n" << endl;
        return -1;
    }

    // Read in each frame for storage and processing 
    while(input_cap.read(frame) ){
        if(frames.size() >= limit){
	    	frames.pop();
            frames.push(frame.clone());
        } else {
            frames.push(frame.clone());
        }

        // Open file to write signal data
        signals.open("/home/ubuntu/AngryBirds/signals.txt",
                      fstream::in  |
                      fstream::out |
                      fstream::app);

        // Basic test condition (for testing purposes - to be 
        // changed). Flag if particular signal exceeds test threshold
        // otherwise, proceed with continuous footage capture 
        if ((test_adc->getNumericValue() > TEST_THRESHOLD) ||
            (test_count >= max_count)){
            if (test_adc->getNumericValue() > TEST_THRESHOLD){
                 detected = true;
                 collision = true;
                 signals << ("\n" + get_date() + ":    ");
                 signals << test_adc->getNumericValue();
                 signals << "\n";
             }
            save = true;
	    	limit = POSTTIME;
        }

		#ifdef DEBUG
        cout << "TEST_COUNT: " << test_count << endl;
        cout << "FRAME SIZE: " << frames.size() << endl;
        cout << "SAVE: " << save << endl;
		#endif

        // Event detected, save queue to write to output file
        if((save) && (frames.size() >= limit)){
			#ifdef DEBUG
            if (detected){
            	cout << "\nEVENT TRIGGERED!\n" << endl;
                collision = true;
            } else {
            	cout << "\nSAVING SCHEDULED NON-COLLISON CLIP\n" << endl;
            }
			#endif
            if (detected) { collision = true; }

            #ifdef DEBUG
			cout << "\nCREATING VIDEO\n" << endl;
			#endif

       	    // Create the output destination. Check if directory
            // already exists before creating new directory
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

            #ifdef DEBUG
			cout << "\nPUSHING FRAMES\n" << endl;
	 		#endif

            // Create the thread that will update the queue of video frames
            // while we are writing the current queue of frames to the ouput
            // file  
	    //!!thread update_thread(update_frames, frames, frame, limit); 
            thread_ret = pthread_create(&update_thread,
                                        NULL,
                                        update_frames,
                                        (void*)&update_args);

            cout << "\nTHREAD RET: \n" << thread_ret << endl;

            // Check whether we can create the new thread
            if (thread_ret)
            {
                cout << "\nERROR CREATING THREAD\n" << endl;
                exit(EXIT_FAILURE);
            }

            // Write collision seqeuence to output file
            while(!frames.empty()){
                #ifdef DEBUG
			cout << "WRITING FRAME: " << frame_count << endl;
		#endif
                output_cap.write(frames.front());
                frames.pop();
                frame_count += 1;
            }

            // What if ... writing operations takes longer reading operations ?
            // videoWriter's pushing/popping is slower than frame capture's 
            // pusing/popping ... what will happen ? 
            pthread_join(update_thread, NULL);
            frame_count = 0;

            #ifdef DEBUG
			cout << "\nDONE WRITING\n" << endl;
			#endif

            test_count = 0;
            collision = false;
            save = false;
	    	limit = PRETIME;
            output_cap.release();
         }
        test_count++;
	//We need to close the signals in order to write to the
	//file.
        signals.close();
    }
    input_cap.release();
}



/*---------------------------------------------------------
                  FUNCTION DEFINITIONS
  ---------------------------------------------------------*/
/* For testing purposes ...
void print_hello(string msg) 
{
    int i = 0;
    while (i < 100) 
    {
        cout << "\n" + msg + "\n" << endl; 
    }
}
*/


/* Description: Creates a new directory to store footage if
 *              it doesn't exist already
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

/* Description: Returns the current system time
 * Note: time portion of datetime stamp is formated as
 *       HOUR_MINUTE_SEC because colons are considered a invalid
 *       character in naming files.
 */
string get_date()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%F %H_%M_%S", timeinfo);
    return(string(buffer));
}


/* Description: Creates a new ID to name output video file
 *              in format "Year-Month-Day Hour_Minute_Second"
 */
string create_id(const char *path, bool collision)
{
    if (collision)
    {
        return (path + get_date() + ".avi");
    }
    // No Collision
    return( path + get_date() + "_NC" + ".avi");
}


/* Description: 
 */
//!!void update_frames(queue<Mat> &frames, Mat &frame, int limit)
void *update_frames(void *update_args)
{
    cout << "\nINSIDE UPDATE FRAMES\n" << endl;
    struct update_struct *args = (struct update_struct*)update_args;

    if((args->frames).size() >= (args->limit))
    {
	(args->frames).pop();
        (args->frames).push((args->frame).clone());
    }
    else
    {
        (args->frames).push((args->frame).clone());
    }
}

//-----EOF-----

