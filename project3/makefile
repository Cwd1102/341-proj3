CXX = g++
CXXFlags = -Wall

proj3: pqueue.o driver.cpp
	$(CXX) $(CXXFlags) pqueue.o driver.cpp -o proj3
pqueue.o: pqueue.cpp pqueue.h 
	$(CXX) $(CXXFlags) -c pqueue.cpp