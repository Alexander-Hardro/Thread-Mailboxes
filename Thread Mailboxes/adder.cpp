#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include "mailbox.h"


/// the worker thread function, each thread will use this to process the messages sent to its "mailbox"
void* adder(void* arg){
    /// thread ID
    int id = *(int*)arg;
    /// accumulated sum
    int sum = 0;
    /// number of operations
    int count = 0;
    /// wether the first message has been sent or not
    bool started = false;
    /// keeps track of the starting time the thread is being worked on
    time_t start = time(nullptr);


    while (true){
        msg m;
        /// waits for message
        recieveMsg(id, m);

        /// starts the timer after the first message is sent
        if(!started && m.value >= 0){
            start = time(nullptr);
            started = true;
        }

        /// terminates if an invalid value is entered
        if(m.value < 0){
            break;
        }
        sum += m.value;
        count++;
        sleep(1);
    }

    time_t end = time(nullptr);

    /// assigns the values to proper variables for output later on
    msg reply;
    reply.iFrom = id;
    reply.value = sum;
    reply.cnt = count;
    reply.tot = started ? (int)(end-start) : 0;

    /// sends results to main thread
    sendMsg(0, reply);
    pthread_exit(nullptr);
}