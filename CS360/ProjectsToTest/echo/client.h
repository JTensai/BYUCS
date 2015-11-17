#pragma once

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <sstream>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Client {
public:
    Client(string host, int port, bool debug);
    ~Client();

    void run();

private:
    virtual void create();
    virtual void close_socket();
    void echo();
    bool send_request(string);
    string get_response();
    vector<string> split(string, char);
    void handle_send(string, string);
    void handle_list(string);
    void handle_read(string, int);

    string host_;
    int port_;
    int server_;
    int buflen_;
    char* buf_;
    bool debug;
};
