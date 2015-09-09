#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

// #include <fstream>

using namespace std;

string cache = "";

int open_socket(string host, int port){

  struct hostent *hostEntry;
  hostEntry = gethostbyname(host.c_str());
  if (!hostEntry) {
      cout << "No such host name: " << host << endl;
      exit(-1);
  }

  struct sockaddr_in server_addr;
  memset(&server_addr,0,sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);


  int server = socket(PF_INET,SOCK_STREAM,0);
  if (server < 0) {
      perror("socket");
      exit(-1);
  }

  if (connect(server,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
    perror("connect");
    exit(-1);
  }
  return server;
}

string read_message(){
  // # read until we have a newline and store excess in cache
  int index = cache.find("\n");
  if (index == -1){
    return "";
  }

  string message = cache.substr(0, index+1);
  cache = cache.substr(index+1);
  return message;
}

string get_response(int& server){
  // # get a response from the server
  string message;
  int buflen = 1024;
  char* buf = new char[buflen+1];
  while (1){

    memset(buf,0,buflen);
    int returned = recv(server,buf,buflen,0);
    if (returned > 0){
      cache += string(buf);
      message = read_message();

      if (message == "")
        continue;
      else{
        delete[] buf;
        return message;
      }
    }
    else{
      delete[] buf;
      return "No response";
    }
  }
  return "";
}







/* Handling Send */
void send_request(string request, int& server){
  send(server, request.c_str(), request.length(), 0);
}

void send_put(string name, string subject, string message, int& server){
  stringstream ss;
  ss << "put " << name << " " << subject << " " << message.length() << "\n" << message;
  send_request(ss.str(), server);
}

void response_to_put(int& server){
    string message = get_response(server);
    if (message != "OK\n")
      cout << "Server returned bad message from put :" << message << ":" << endl;
}

string get_user_message(){
  string msg;
  vector<string> lines;
  cout << "- Type your message. End with a blank line -" << endl;
  while (getline(cin, msg)){
    if (msg.length() == 0){
      break;
    }
    else{
      lines.push_back(msg);
    }
  }
  stringstream ss;

  for(string s : lines){
    ss << s << endl;
  }

  return ss.str();
}





/* Handling List */

void send_list(string name, int& server){
  stringstream ss;
  ss << "list " << name << "\n";
  send_request(ss.str(), server);
}

void read_list_response(int num){
  int total = 0;
  while (total < num){
    string data = read_message();
    cout << data;
    total++;
  }
}

void response_to_list(int&server){
  string message = get_response(server);

  vector<string> tokens;
  stringstream ss;
  for(char c : message){
    if (c == ' '){
      tokens.push_back(ss.str());
      ss.str("");
    }
    else{
      ss << c;
    }
  }
  tokens.push_back(ss.str());

  int num;
  if (tokens.size() == 2){
    if (tokens[0] != "list"){
      message = "Server returned bad message:" + message;
      cout << message << endl; 
    }
    num = stoi(tokens[1]);
  }
  else{
    message = "Server returned bad message:" + message;
    cout << message << endl;    
  }

  read_list_response(num);
}





/* Handling Read */

void send_read(string name, int index, int& server){
  stringstream ss;
  ss << "get " << name << " " << index << "\n";
  send_request(ss.str(), server);
}

void read_message_response(string subject, int length, int&server){
  string data = cache;

  string message;
  int buflen = 1024;
  char* buf = new char[buflen+1];


  while (data.size() < length){
    int returned = recv(server,buf,buflen,0);


    if (returned > 0){
      data += string(buf);

      if (message == "")
        continue;
      else{
        delete[] buf;
        return;// message;
      }
    }
    else{
      cache = "";
      cout << "Server did not send the whole message:" << endl;
    }
  }
  if (data.size() > length){
    cache = data.substr(length);
    data = data.substr(0,length);
  }
  else{
      cache = "";
  }
  cout << subject << endl;
  cout << data;
  delete[] buf;
}
void response_to_read(int& server){
  string message = get_response(server);

  vector<string> tokens;
  stringstream ss;
  for(char c : message){
    if (c == ' '){
      tokens.push_back(ss.str());
      ss.str("");
    }
    else{
      ss << c;
    }
  }
  tokens.push_back(ss.str());

  string subject;
  int length;
  if (tokens.size() == 3){
    if (tokens[0] != "message"){
      message = "Server returned bad message:" + message;
      cout << message << endl; 
    }
    subject = tokens[1];
    length = atoi(tokens[2].c_str());
    read_message_response(subject, length, server);
  }
  else{
    message = "Server returned bad message:" + message;
    cout << message << endl;    
  }

}




/* Handling Reset */

// void send_reset(int& server){
//   stringstream ss;
//   ss << "reset" << "\n";
//   send_request(ss.str(), server);
// }

// void response_to_reset(int& server){
//     string message = get_response(server);
//     if (message != "OK\n")
//       cout << "Server returned bad message:" << message << endl;
// }








int parse_command(string command, int& server){
  vector<string> tokens;
  stringstream ss;
  for(char c : command){
    if (c == ' '){
      tokens.push_back(ss.str());
      ss.str("");
    }
    else{
      ss << c;
    }
  }
  tokens.push_back(ss.str());


  if (tokens[0] == "quit"){
    return -1;
  }
  if (tokens[0] == "send"){
    // ### send message ###
    if (tokens.size() == 3){
      string name = tokens[1];
      string subject = tokens[2];
      string message = get_user_message();
      send_put(name, subject, message, server);
      response_to_put(server);
      return 1;
    }
    else
      return 0;
  }
  if (tokens[0] == "list"){
    // ### list messages ###
    if (tokens.size() == 2){
      string name = tokens[1];
      send_list(name, server);
      response_to_list(server);
      return 1;
    }
    else
      return 0;
  }
  if (tokens[0] == "read"){
    // ### read message ###
    if (tokens.size() == 3){
      string name = tokens[1];
      int index = atoi(tokens[2].c_str());
      send_read(name, index, server);
      response_to_read(server);
      return 1;
    }
    else
      return 0;
  }
  // if (tokens[0] == "reset"){
  //   // ### reset messages ###
  //   if (tokens.size() == 1){
  //     send_reset(server);
  //     response_to_reset(server);
  //     return 1;
  //   }
  //   else
  //     return 0;
  // }
  else
    return 0;
}

int main(int argc, char* argv[]){
  // setup default arguments
  int option;
  int port = 5000;
  string host = "localhost";

  while ((option = getopt(argc,argv,"h:p:")) != -1) {
    switch (option) {
      case 'p':
        port = atoi(optarg);
        break;
      case 'h':
        host = optarg;
        break;
      default:
        cout << "client [-s IP address] [-p port]" << endl;
        exit(EXIT_FAILURE);
    }
  }

  int server = open_socket(host, port);


  while (1){
    // get input from users
    cout << "% ";
    string command;
    getline(cin, command);
    int result = parse_command(command, server);
    if (result == 0){
      cout << "I don't recognize that command." << endl;
    }
    if (result == -1){
      break;
    }
  }

  return 0;
}