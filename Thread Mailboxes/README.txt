Project 3 Read Me 
Alexander Hardro
2/18/26


The following program is meant to simulate a mailbox system by establishing a set of threads that then send integers to one another in a format determined by user input. The program uses semaphores so the data of the threads is accessed one at a time. Each thread has one message, and once all the messages have been sent, the program prints out the resulting number of all the messages for each thread, the operations, and the time period it took to perform all the operations of each thread.


How to compile:


The program is compiled using the Makefile. Type “make clean” and then “make” into the terminal while in the directory to compile the program


The program can run in two ways depending on the command arguments below:


1. ./mailbox int = runs the program with several threads equal to the entered integer. After giving the commands above, the user continuously sends messages to each mailbox by entering two integers, the first signifying the amount it wants to send, and the second integer is the thread it wants to send that number to. To stop sending messages and print the results, type “eof” to signify the end of input. 
 
2. ./mailbox int nb = same as running with the command arguments above, except it tells the program to use NBSendMsg() to send messages between threads. Again, type “eof” to see the results of each thread.