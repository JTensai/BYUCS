# Makefile for echo client and server

CXX=			g++ -std=c++11 $(CCFLAGS)

SERVER =		messageServer
CLIENT =		messageClient
OBJS =			$(SERVER) $(CLIENT)

CCFLAGS= -g

all:	$(SERVER) $(CLIENT)

$(SERVER):$(SERVER).cpp
	$(CXX) -o $(SERVER) $(SERVER).cpp

$(CLIENT):$(CLIENT).cpp
	$(CXX) -o $(CLIENT) $(CLIENT).cpp

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(SERVER) $(CLIENT)
