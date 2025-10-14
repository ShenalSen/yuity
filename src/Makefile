CC = g++
CFLAGS = -Wall -g
OBJS = main.o vehicle.o user.o sales.o

all: tourmate

tourmate: $(OBJS)
	$(CC) $(CFLAGS) -o tourmate $(OBJS)

main.o: main.cpp vehicle.h user.h sales.h
	$(CC) $(CFLAGS) -c main.cpp

vehicle.o: vehicle.cpp vehicle.h
	$(CC) $(CFLAGS) -c vehicle.cpp

user.o: user.cpp user.h
	$(CC) $(CFLAGS) -c user.cpp

sales.o: sales.cpp sales.h vehicle.h
	$(CC) $(CFLAGS) -c sales.cpp

clean:
	del *.o tourmate.exe