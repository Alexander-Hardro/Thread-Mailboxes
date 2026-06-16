CXX = g++
CXXFLAGS = -pthread -Wall

all: mailbox

mailbox: main.o mailbox.o adder.o
	$(CXX) $(CXXFLAGS) -o mailbox main.o mailbox.o adder.o

main.o: main.cpp mailbox.h
	$(CXX) $(CXXFLAGS) -c main.cpp

mailbox.o: mailbox.cpp mailbox.h
	$(CXX) $(CXXFLAGS) -c mailbox.cpp

adder.o: adder.cpp mailbox.h
	$(CXX) $(CXXFLAGS) -c adder.cpp

clean:
	rm -f *.o mailbox
