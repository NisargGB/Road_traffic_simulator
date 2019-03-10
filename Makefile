objects = Vehicle.o simulation.o
CC = g++
CFLAGS = -Wall -c 

edit : $(objects)
						$(CC) $(objects) -o COP_Simulator

Vehicle.o : Vehicle.cpp
						$(CC) $(CFLAGS) Vehicle.cpp
simulation.o : simulation.cpp
						$(CC) $(CFLAGS) simulation.cpp


clean :
						rm edit $(objects)
