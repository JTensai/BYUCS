#include <iostream>
#include <sstream>
#include <string>

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

using namespace std;


class Server{
  private:
    struct sockaddr_in server_addr;
    int server_socket;

  public:
    Server(){}

    Server(string host, int port){
      // setup socket address structure
      memset(&server_addr,0,sizeof(server_addr));
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(port);
      server_addr.sin_addr.s_addr = INADDR_ANY;

      // create socket
      int server = socket(PF_INET,SOCK_STREAM,0);
      if (!server) {
          perror("socket");
          exit(-1);
      }

      // set socket to immediately reuse port when the application closes
      int reuse = 1;
      if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
          perror("setsockopt");
          exit(-1);
      }

      // call bind to associate the socket with our local address and port
      if (bind(server,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
          perror("bind");
          exit(-1);
      }

      // convert the socket to listen for incoming connections
      if (listen(server,SOMAXCONN) < 0) {
          perror("listen");
          exit(-1);
      }
      server_socket = server;
    }

    int get_server_socket(){
      return server_socket;
    }

};