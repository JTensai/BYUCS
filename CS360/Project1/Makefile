# Makefile for echo client and server

CXX=			g++ $(CCFLAGS)

SERVER =		messageServer.o
CLIENT =		messageClient.o
OBJS =			$(SERVER) $(CLIENT)

LIBS=

CCFLAGS= -g

all:	messageServer messageClient

messageServer:$(SERVER)
		$(CXX) -o messageServer $(SERVER) $(LIBS)

messageClient:$(CLIENT)
		$(CXX) -o messageClient $(CLENT) $(LIBS)

clean:
		rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
		rm -f $(OBJS) $(OBJS:.o=.d) messageServer messageClient