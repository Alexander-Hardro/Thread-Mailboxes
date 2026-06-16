#include <iostream>
#include <pthread.h>
#include <queue>
#include <vector>
#include <cstdlib>
#include "mailbox.h"

using namespace std;

extern void* adder(void*);

/// main thread which creates worker threads, reads input from cin, 
/// sends messages to the other threads, and collets/prints results
int main(int argc, char* argv[]) {
    const int MAXTHREAD = 10;
    /// checks that valid command arguements were given
    if (argc < 2) {
        cerr << "Usage: mailbox <numThreads> [nb]\n";
        return 1;
    }
    int numThreads = atoi(argv[1]);
    /// checks if a valid number of threads were given
    if (numThreads < 1 || numThreads > MAXTHREAD) {
        cerr << "Incorrect number of threads given\n";
        return 1;
    }
    /// checks if we are running in non blocking mode
    bool usingNB = (argc == 3 && string(argv[2]) == "nb");

    initiateMailbox(numThreads);

    pthread_t threads[MAXTHREAD];
    int ids[MAXTHREAD];

    /// creates worker threads
    for (int i = 1; i <= numThreads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], nullptr, adder, &ids[i]);
    }

    /// que for messages hat failed NB send
    queue<msg> undelivered;

    /// tracks outstanding messaged per thread (non blocking mode)
    vector<int> pending(numThreads + 1, 0);

    int value, target;

    /// reads input until eof is entered
    while (cin >> value >> target) {
        /// checks if the entered message is valid
        if (value < 0 || target < 1 || target > numThreads) {
            cerr << "Invalid input skipped\n";
            continue;
        }
    
        msg m{0, value, 0, 0};

        if (!usingNB) {
            // BLOCKING MODE
            sendMsg(target, m);
        }
        else {
            // NON-BLOCKING MODE
            if (NBSendMsg(target, m) != 0) {
                m.iFrom = target;
                undelivered.push(m);
                pending[target]++;
            }
        }
    }

    // Retry undelivered messages (NB mode only)
    if (usingNB) {
        while (!undelivered.empty()) {
            msg m = undelivered.front();
            if (NBSendMsg(m.iFrom, m) == 0) {
                undelivered.pop();
                pending[m.iFrom]--;
            }
        }
    }

    // Send termination messages
    if (!usingNB) {
        // Blocking termination
        for (int i = 1; i <= numThreads; i++) {
            msg term{0, -1, 0, 0};
            sendMsg(i, term);
        }
    }
    else {
        // NB termination — only after all messages delivered
        vector<bool> terminated(numThreads + 1, false);

        while (true) {
            bool done = true;

            for (int i = 1; i <= numThreads; i++) {
                if (!terminated[i] && pending[i] == 0) {
                    msg term{0, -1, 0, 0};
                    if (NBSendMsg(i, term) == 0) {
                        terminated[i] = true;
                    }
                }
                if (!terminated[i])
                    done = false;
            }

            if (done)
                break;
        }
    }

    // Receive and print results
    for (int i = 1; i <= numThreads; i++) {
        msg r;
        recieveMsg(0, r);
        cout << "The result from thread " << r.iFrom
             << " is " << r.value
             << " from " << r.cnt
             << " operations during " << r.tot
             << " seconds.\n";
    }

    /// waits for all threads to finish
    for (int i = 1; i <= numThreads; i++) {
        pthread_join(threads[i], nullptr);
    }

    return 0;
}
