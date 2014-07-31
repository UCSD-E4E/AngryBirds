#ifndef SENSOR_SIGNAL_H
#define SENSOR_SIGNAL_H

#define SAMPLE_SIZE 0 //!! Need to change

#include <deque>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "BlackLib.h"

using namespace std;

class SensorSignal
{
    private:
        deque<int> sensor_signal;

    public:
        SensorSignal();
        void build_signal_deque(deque<int> &signal_deque, 
                                const int sample_size);
        float compute_average_signal(deque<int> &signal_deque, 
                                     const int sample_size);
        float compute_normal_signal(deque<int> &signal_deque, 
                                    const int average_signal, 
                                    const int sample_size);
         
};
#endif // EOF


