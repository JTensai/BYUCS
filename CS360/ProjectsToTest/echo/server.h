#pragma once

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>

#include <string>
#include "message.h"

using namespace std;

class Server {
public:
    Server(int port, bool debug);
    ~Server();

    void run();
    
private:
    void create();
    void close_socket();
    void serve();
    void handle(int);
    string get_request(int);
    bool send_response(int, string);
    vector<string> split(string, char);
    string read_message(int, int);
    bool check_put(string);
    

    int port_;
    int server_;
    int buflen_;
    char* buf_;
    map<string, vector<Message> > cache;
    bool debug;
};
