/*
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <thread>

using namespace std;

// The function we want to make the thread run.
void task1(string msg)
{
    cout << "task1 says: " << msg;
}

int main()
{
    // Constructs the new thread and runs it. Does not block execution.
    thread t1(task1, "Hello");

    // Makes the main thread wait for the new thread to finish execution, i
    // therefore blocks its own execution.
    t1.join();
}
*/

#include <stdio.h>
#include <pthread.h>

struct arg_struct {
    int arg1;
    int arg2;
};

void *print_the_arguments(void *arguments)
{
    //struct arg_struct *args = (struct arg_struct *)args;
    struct arg_struct *args = (struct arg_struct *)arguments;
    printf("%d\n", args -> arg1);
    printf("%d\n", args -> arg2);
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    pthread_t some_thread;
    struct arg_struct args;
    args.arg1 = 5;
    args.arg2 = 7;

    if (pthread_create(&some_thread, NULL, &print_the_arguments, (void *)&args) != 0) {
    	printf("Uh-oh!\n");
    	return -1;
    }

    return pthread_join(some_thread, NULL); /* Wait until thread is finished */
}
