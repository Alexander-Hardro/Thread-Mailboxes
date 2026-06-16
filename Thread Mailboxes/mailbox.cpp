#include "mailbox.h"
#include <vector>

using namespace std;

/// a vector of threads
static vector<msg> mailboxes;
/// a semaphore indicating if a mailbox is empty (1 if empty)
static vector<sem_t> semEmpty;
/// a semaphore indicating whether a mailbox has a message (1 if full)
static vector<sem_t> semFull;

/// initialize mailboxes and semaphores
/// mailboces start empty, so semEmpty=1 and semFull=0
void initiateMailbox(int n) {
    mailboxes.resize(n + 1);
    semEmpty.resize(n + 1);
    semFull.resize(n + 1);

    for (int i = 0; i <= n; i++) {
        sem_init(&semEmpty[i], 0, 1);  // mailbox starts empty
        sem_init(&semFull[i], 0, 0);   // no message available
    }
}

/// waits until mailbox is empty, sotres the message, then signals reciever
void sendMsg(int to, msg &m) {
    sem_wait(&semEmpty[to]);
    mailboxes[to] = m;
    sem_post(&semFull[to]);
}

/// attempts to require empty slot, and returns -1 if mailbox is full
int NBSendMsg(int to, msg &m) {
    if (sem_trywait(&semEmpty[to]) != 0)
        return -1;

    mailboxes[to] = m;
    sem_post(&semFull[to]);
    return 0;
}

/// waits for a message to exist, than copies it, removes it, and makes the mailbox empty
void recieveMsg(int me, msg &m) {
    sem_wait(&semFull[me]);
    m = mailboxes[me];
    sem_post(&semEmpty[me]);
}
