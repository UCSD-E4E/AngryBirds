/* Filename: SensorSignal.cpp
 * Author: Angela To
 * Date: 8/1/2014
 * Description: Helper functions to classify analog waveform readings
 */

#include "SensorSignal.h"
#include "BlackLib.h"

using namespace std;


/*
 *
 */
SensorSignal::SensorSignal() 
{ 
    //empty constructor// 
}



/* Builds and maintains a continuous buffer for storing and 
 * processing analog waveform signals 
 *
 * ADC Ground = pin 34
 * AIN0 = pin 39
 * AIN1 = pin 40
 */
void SensorSignal::build_signal_deque(deque<int> &signal_deque,
                                      const int window_size) 
{
    BlackADC* adc = new BlackADC(AIN0);
    if (signal_deque.size() > window_size) 
    {
        signal_deque.pop_back();
        signal_deque.push_front(adc-> getNumericValue());
    }
    else 
    {
        signal_deque.push_front(adc-> getNumericValue());
    }
}


/* Computes the averaged analog signal based on a predetermined 
 * (ground truth) sample size
 */
float SensorSignal::compute_average_signal(deque<int> &signal_deque, 
                                           const int window_size)
{
    float sum;
    float average;
    for (deque<int>::iterator itr = signal_deque.begin(); itr != signal_deque.end(); ++itr) 
    {
        sum += *itr;
    }
    average = (sum / window_size);
    return average;
}


/* Computes the normalized analog signal in which we can use to 
 * compare against a predetermined (ground truth) threshold value
 */
float SensorSignal::compute_normal_signal(deque <int> &signal_deque,
                                          deque <int> &averaged_deque,
                                          const int window_size) 
{
    float sum;
    float difference;
    float normalized_signal;  
    
    deque<int>::iterator s_itr = signal_deque.begin();
    deque<int>::iterator a_itr = averaged_deque.begin();

    for (; s_itr != signal_deque.end(); ++s_itr, ++a_itr) 
    {
        difference = (*s_itr - *a_itr);
        sum += difference;
    }
    normalized_signal = (sum / window_size);
}

