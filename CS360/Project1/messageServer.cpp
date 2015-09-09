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


string read_put(int length, int client){
  string data = cache;
  while (data.size() < length){
    int d = recv(client,buf,buflen,0);
    if (d < 0)
      return "";
    data += string(buf);
  }
  // cout << data.size() << " > " << length << endl;
  if (data.size() > length){
    cache = data.substr(length);
    // cout << "read_put cache: " << cache;
    data = data.substr(0,length);
  }
  // cout << "data from read_put : " << data << endl;
  return data;
}

void store_message(string name, string subject, string message){
  vector<pair<string, string>> vector_of_messages;

  if (message_box.count(name) > 0){
    vector_of_messages = message_box[name];
  }
  vector_of_messages.push_back(make_pair(subject, message));
  message_box[name] = vector_of_messages;
  cache = "";
}

void print_the_message_box(){
  cout << "All Mailbox Contents:" << endl;

  for (auto& entry : message_box){
    cout << entry.first << endl;
    for (auto& entry2 : entry.second){
      cout << "\t" << entry2.first << " : " << entry2.second << endl;
    }
  }
}

string create_list_results(string name){
  if (message_box[name].size() > 0){
    stringstream ss1, ss2;
    int i = 0;
    vector<pair<string, string>> vector_of_messages = message_box[name];
    for(pair<string, string> p : vector_of_messages){
      ss1 << ++i << " " << p.first << endl;
    }
    ss2 << "list " << i << "\n" << ss1.str();
    return ss2.str();
  }
  else{
    return "list 0\n";
  }

}

string create_read_result(string name, int index){
  // cout << name  << " : " << index << endl;
  // print_the_message_box();
  pair<string, string> message;
  if (message_box[name].size() > 0){
    vector<pair<string, string>> vector_of_messages = message_box[name];
    if (index > vector_of_messages.size())
      return "error no such message for that user\n";
    message = vector_of_messages[index-1];

    string subject = message.first;
    string the_message = message.second;
    int length = the_message.size();
    stringstream ss;
    ss << "message " << subject << " " << length << '\n' << the_message;// << endl;
    // cout << ":"<< "message " << subject << " " << length << '\n' << the_message << ":" << endl;
    return ss.str();
  }
  return "error no such message for that user\n";
}

string create_response(string message, int client){
  vector<string> tokens;
  stringstream ss;
  for(char c : message){
    if (c == ' '){
      tokens.push_back(ss.str());
      ss.str("");
    }
    else{
      if (c != '\n')
        ss << c;
    }
  }
  tokens.push_back(ss.str());

  string response;

  if (tokens.empty()){
    return("error invalid message (empty)\n");
  }
  if (tokens[0] == "reset"){
    message_box.clear();
    return "OK\n";
  }
  if (tokens[0] == "put"){
    if (tokens.size() == 4){
      string name = tokens[1];
      string subject = tokens[2];
      int length = atoi(tokens[3].c_str());
      
      string message = read_put(length, client);
      if (message == ""){
        return "error could not read entire message";
      }
      store_message(name,subject,message);
      return "OK\n";
    }
    else{
      return "error invalid message (put)\n";
    }
  }
  if (tokens[0] == "list"){
    // memset(buf, 0, sizeof(buf));
    // cache = "";
    if (tokens.size() == 2){
      string name = tokens[1];

      response = create_list_results(name);
      return response;
    }
    else{
      return "error invalid message (list)\n";
    }
  }
  if (tokens[0] == "get"){
    if (tokens.size() == 3){
      string name = tokens[1];
      int index = atoi(tokens[2].c_str());

      string response = create_read_result(name, index);
      return response;
    }
    else{
      return "error invalid message (get)\n";
    }
  }
  return "error invalid message (overall)\n";
}

string get_request(int client, int& nread) {
    string receieved = "";
    // read until we get a newline   
    while (receieved.find("\n") == string::npos) {
      nread = recv(client,buf,buflen,0);
      if (nread < 0) {
        if (errno == EINTR)
          // the socket call was interrupted -- try again
          continue;
        else
          // an error occurred, so break out
          return "";
      } 
      else if (nread == 0) {
        // the socket is closed
        return "";
      }
      // be sure to use append in case we have binary data
      receieved.append(buf,nread);

    }

    int index = receieved.find('\n');
    if (index == -1){
      return "";
    }

    // a better server would cut off anything after the newline and
    // save it in a cache
    string request = receieved.substr(0, index+1);
    cache = receieved.substr(index+1);


    return request;
}

bool send_response(int client, string response) {
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;

    // loop to be sure it is all sent
    while (nleft) {
      nwritten = send(client, ptr, nleft, 0);
      // cout << nwritten << " :" << response << ":" << endl;
      if (nwritten < 0) {
        if (errno == EINTR) {
          // the socket call was interrupted -- try again
          continue;
        } else {
          // an error occurred, so break out
          perror("write");
          return false;
        }
      } else if (nwritten == 0) {
        // the socket is closed
        return false;
      }
      nleft -= nwritten;
      ptr += nwritten;
    }
    return true;
}

int handle_client(int& client, int& nread){
  while (1) {
    string request = get_request(client, nread);
    // cout << "Request: " << request;

    if (request.empty()){
      close(client);
      return 0;
    }

    string response = create_response(request, client);

    bool success = send_response(client,response);
    // break if an error occurred
    if (!success){
      close(client);
      return 0;
    }
  }
}


int main(int argc, char* argv[]){
  // setup default arguments
  int option;
  int port = 5000;
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