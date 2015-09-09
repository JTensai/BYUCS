#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <mutex>

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "Worker.h"
#include "Server.h"

using namespace std;

bool debug = false;
MessageBox message_box;
ClientBuffer client_buffer;  
Server server;

void print_debug(string s, string desc){
  if (debug){
    stringstream ss;
    // ss << "\e[0;31m";
    ss << "DEBUG: " << desc << endl;
    ss << ":" << s << ":" << endl;
    // ss << "\e[0m";
    cout << ss.str();
  }
}
void print_debug(string s){
  if (debug){
    stringstream ss;
    // ss << "\e[0;31m";
    ss << "DEBUG:" << s << ":" << endl;
    // ss << "\e[0m";
    cout << ss.str();
  }
}



void* start_thread(void* args){
  // just keeps looping through this while the thread exists
  // should check queue to see if there is a client and take it
  Worker* w = (Worker*) args;
  while (1){
    int client = client_buffer.take();
    if (debug){
      stringstream ss;
      ss << "\nCLIENT FOUND!" << endl;
      ss << "Client (" << client << ") claimed by thread " << w->get_num() << endl;
      cout << ss.str();
    }
    w->handle(client);
    if (debug){
      stringstream ss;
      ss << "\n\tClient (" << client << ") dealth with, moving on." << endl;
      cout << ss.str();
    }
  }
}

int main(int argc, char* argv[]){
  // setup default arguments
  int option;
  int port = 5000;
  string host = "localhost";
  while ((option = getopt(argc,argv,"p::d")) != -1) {
    switch (option) {
      case 'p':
        port = atoi(optarg);
        break;
      case 'd':
        debug = true;
        break;
      default:
        cout << "client [-s IP address] [-p port]" << endl;
        exit(EXIT_FAILURE);
    }
  }

  server = Server(host, port);
  struct sockaddr_in client_addr;
  socklen_t clientlen = sizeof(client_addr);
  int nume_of_threads = 10;
  pthread_t threads[nume_of_threads];


  // create our 10 threads
  for (int i = 0; i < nume_of_threads; i++) {
    Worker* w = new Worker(message_box, client_buffer, i);
    pthread_create(&threads[i], NULL, start_thread, (void *) w);
  }


  int client;
  while (client = accept(server.get_server_socket(), (struct sockaddr *)&client_addr, &clientlen)) {
    client_buffer.append(client);
  }
  

  return 0;
}