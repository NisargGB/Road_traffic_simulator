objects = main.o
CC = g++
CFLAGS = -Wall -c

edit : $(objects)
						$(CC) $(objects) -o COP_Simulator -lglu32 -lopengl32 -lfreeglut -lglew32

main.o : main.cpp
						$(CC) $(CFLAGS) main.cpp
clean :
						rm edit $(objects)
