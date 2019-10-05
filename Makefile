objects = Vehicle.o simulation.o updateGrid.o updatePosition.o updateVelocity.o printLayout.o check_lane_exchange.o  main.o
CC = g++
CFLAGS = -Wall -c

edit : $(objects)
						$(CC) $(objects) -o COP_Simulator -lglu32 -lopengl32 -lfreeglut -lglew32 -pthread
						
Vehicle.o : Vehicle.cpp functions.h
						$(CC) $(CFLAGS) Vehicle.cpp
simulation.o : simulation.cpp functions.h
						$(CC) $(CFLAGS) simulation.cpp
printLayout.o : printLayout.cpp functions.h
						$(CC) $(CFLAGS) printLayout.cpp
updateGrid.o : updateGrid.cpp functions.h
						$(CC) $(CFLAGS) updateGrid.cpp
updatePosition.o : updatePosition.cpp functions.h
						$(CC) $(CFLAGS) updatePosition.cpp
updateVelocity.o : updateVelocity.cpp functions.h
						$(CC) $(CFLAGS) updateVelocity.cpp
check_lane_exchange.cpp : check_lane_exchange.cpp functions.h
						$(CC) $(CFLAGS) check_lane_exchange.cpp
main.o : main.cpp functions.h
								$(CC) $(CFLAGS) main.cpp

clean :
						rm edit $(objects)
