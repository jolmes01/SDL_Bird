SDLFLAGS := $(shell sdl2-config --libs --cflags)

all:flappyCliente flappy server

server:SocketDatagrama.o PaqueteDatagrama.o
	g++ serverFlappy.cpp SocketDatagrama.o PaqueteDatagrama.o -o server

flappyCliente:SocketDatagrama.o PaqueteDatagrama.o
	g++ flappyCliente.cpp SocketDatagrama.o PaqueteDatagrama.o $(SDLFLAGS) -o flappyCliente

flappy: SocketDatagrama.o PaqueteDatagrama.o
	g++ flappy.cpp $(SDLFLAGS) -o flappy

SocketDatagrama.o: SocketDatagrama.cpp SocketDatagrama.h
	g++ SocketDatagrama.cpp -c 

PaqueteDatagrama.o: PaqueteDatagrama.cpp PaqueteDatagrama.h
	g++ PaqueteDatagrama.cpp -c

clean:
	rm *.o flappy flappyCliente server
