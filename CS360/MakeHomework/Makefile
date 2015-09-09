# Makefile for echo client and server

CXX=		g++ $(CCFLAGS)

GOODBYE =	goodbye
HELLO =		hello
OBJS =		$(GOODBYE) $(HELLO)

CCFLAGS= -g

all:	$(GOODBYE) $(HELLO)

$(GOODBYE):$(GOODBYE).cc
	$(CXX) -o $(GOODBYE) $(GOODBYE).cc

$(HELLO):$(HELLO).cc
	$(CXX) -o $(HELLO) $(HELLO).cc

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) goodbye hello