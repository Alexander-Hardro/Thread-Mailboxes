#ifndef MAILBOX_H
#define MAILBOX_H

#include <semaphore.h>

#define REQUEST 1
#define REPLY 2

/// The structure for each thread
struct msg {
    int iFrom; /// id of the sender for the thread
    int value; /// the resulting number of all values sent to the thread
    int cnt; /// the number of operations performed for this thread
    int tot; /// time spent processing the thread
};

/// Initializes all mailbox/semaphores
/// n is the number of threads
void initiateMailbox(int n);
/// sends a message to a thread (blocking)
/// to is the value being sent
void sendMsg(int to, msg &m);
/// attempts to send a message (non blocking)
/// to is the value being sent
/// &m is the thread being sent to
int NBSendMsg(int to, msg &m);
/// Copies a message in a thread
/// me is the message
/// &m is the thread recieving the message
void recieveMsg(int me, msg &m);

#endif