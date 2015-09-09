# Makefile for echo client and server

CXX=			g++ -std=c++0x -pthread $(CCFLAGS)

SERVER =		messageServer.o
CLIENT =		messageClient.o
OBJS =			$(SERVER) $(CLIENT)

LIBS=

CCFLAGS= -g

all:	messageServer messageClient

messageServer:$(SERVER)
		$(CXX) -o messageServer $(SERVER) $(LIBS)

messageClient:$(CLIENT)
		$(CXX) -o messageClient $(CLIENT) $(LIBS)

clean:
		rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
		rm -f $(OBJS) $(OBJS:.o=.d) messageServer messageClient