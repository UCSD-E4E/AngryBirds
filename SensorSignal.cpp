/* Filename: SensorSignal.cpp
 * Author: Angela To
 * Date:
 * Description:
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



/*
 *
 */
void SensorSignal::build_signal_deque(deque<int> &signal_deque,
                                             const int sample_size) 
{
    BlackADC* adc = new BlackADC(AIN0);
    if (signal_deque.size() > sample_size) 
    {
        signal_deque.pop_back();
        signal_deque.push_front(adc-> getNumericValue());
    }
    else 
    {
        signal_deque.push_front(adc-> getNumericValue());
    }
}


/*
 *
 */
float SensorSignal::compute_average_signal(deque<int> &signal_deque, 
                                            const int sample_size)
{
    float sum;
    float average;
    for (deque<int>::iterator itr = signal_deque.begin(); itr != signal_deque.end(); ++itr) 
    {
        sum += *itr;
    }
    average = (sum / sample_size);
    return average;
}


/*
 *
 */
float SensorSignal::compute_normal_signal(deque<int> &signal_deque,
                                           const int average_signal,
                                           const int sample_size) 
{
    float sum;
    int difference;
    int normalized_signal;

    for (deque<int>::iterator itr = signal_deque.begin(); itr != signal_deque.end(); ++itr) 
    {
        difference = (*itr - average_signal);
        sum += difference;
    }
    normalized_signal = (sum / sample_size);
}

