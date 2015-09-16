#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

// #include <fstream>

using namespace std;

string cache = "";
int buflen = 1024;
char *buf = new char[buflen+1];
map<string, vector<pair<string, string>>> message_box;

int open_socket(string host, int port, struct sockaddr_in &server_addr){

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
  return server;
}



int handle_client(int& client, int& nread){
    string received = "";
    while (1) {
      nread = recv(client,buf,buflen,0);
      if (nread == 0) {
        break;
      }
      // be sure to use append in case we have binary data
      received.append(buf,nread);
    }



    // store [name] [size]\n
    // message

   //  vector<string> tokens;
	  stringstream ss;
	  while (received.length() > 0){
		  int count = 0;
		  int string_index = 0;
		  string name = "";
		  string length = "";
		  for(char c : received){
		  	if (count > 2){
		  		break;
		  	}
		    if (c == ' ' || c == '\n'){
		    	switch (count){
		    		case 1:
		    			name = ss.str();
		    			break;
		    		case 2:
		    			length = ss.str();
		    			break;
		    	}
		      ss.str("");
		      count++;
		    }
		    else{
		      ss << c;
		    }
		    string_index++;
		  }
		  received.erase(0, string_index + stoi(length));
	  cout << "Stored a file called " << name << " with " << length << " bytes" << endl;
		}
   //  tokens.push_back(ss.str());

	  // string message = cache.substr(0, stoi(tokens[2]));

	  // cache.erase(0,stoi(tokens[2]));

    // cout << "Request: " << request;


	  return 0;
    // if (request.empty()){
    //   close(client);
    //   return 0;
    // }

    // bool success = true;
    // // break if an error occurred
    // if (!success){
    //   close(client);
    //   return 0;
    // }
}


int main(int argc, char* argv[]){
  // setup default arguments
  int option;
  int port = 3000;
  string host = "localhost";
  bool debug = false;

  while ((option = getopt(argc,argv,"h:p:d:")) != -1) {
    switch (option) {
      case 'p':
        port = atoi(optarg);
        break;
      case 'h':
        host = optarg;
        break;
      case 'd':
        debug = true;
        break;
      default:
        cout << "client [-s IP address] [-p port]" << endl;
        exit(EXIT_FAILURE);
    }
  }

  struct sockaddr_in server_addr;
  int server = open_socket(host, port, server_addr);

  int client, nread;
  struct sockaddr_in client_addr;
  socklen_t clientlen = sizeof(client_addr);

  // accept clients
  while ((client = accept(server,(struct sockaddr *)&client_addr,&clientlen)) > 0) {
    // loop to handle all requests
    // cout << "\n\nNEW CLIENT FOUND! : " << client << endl;
    int have_client = 1;
    while (have_client) {
      // read a request
      memset(buf,0,buflen);
      // cout << "Handling client" << endl;
      int temp = handle_client(client, nread);

      send(client, buf, nread, 0);
      have_client = temp;
    }

    // cout << "Broke Free!\n" << endl;
    close(client);
  }
  
  // cout << "Server Shutting Down" << endl;
  close(server);



  return 0;
}