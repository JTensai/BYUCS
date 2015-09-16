# Makefile for echo client and server

CXX=		g++ -std=c++11  $(CCFLAGS)

SERVER =	server
CLIENT =	client
OBJS =		$(SERVER) $(CLIENT)

CCFLAGS= -g

all:	$(SERVER) $(CLIENT)

$(SERVER):$(SERVER).cc
	$(CXX) -o $(SERVER) $(SERVER).cc

$(CLIENT):$(CLIENT).cc
	$(CXX) -o $(CLIENT) $(CLIENT).cc

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) server client