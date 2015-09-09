#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "ClientBuffer.h"
#include "MessageBox.h"

#include <errno.h>

using namespace std;


class Worker{
  private:
    int buflen = 2048;
    char* buf = new char[buflen+1];
    string cache = "";
    int nread, num;
    MessageBox* message_box;
    ClientBuffer* client_buffer;

  public:
    Worker(MessageBox& message_box, ClientBuffer& client_buffer, int i){
      this->message_box = &message_box;
      this->client_buffer = &client_buffer;
    }

    int get_num(){
      return num;
    }




    string read_put(int length, int client){
      // returns message sent with a put command
      string data = cache;
      while (data.size() < length){
        int d = recv(client,buf,buflen,0);
        if (d < 0)
          return "";
        data += string(buf);
      }
      if (data.size() > length){
        cache = data.substr(length);
        data = data.substr(0,length);
      }
      return data;
    }


    string create_response(string request, int client){
      vector<string> tokens;
      stringstream ss;
      for(char c : request){
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
        message_box->clear();
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
          message_box->store_message(name,subject,message);
          return "OK\n";
        }
        else{
          return "error invalid message (put)\n";
        }
      }
      if (tokens[0] == "list"){
        if (tokens.size() == 2){
          string name = tokens[1];
          response = message_box->create_list_results(name);
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

          string response = message_box->create_read_result(name, index);
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

    void handle(int client){
      memset(buf,0,buflen);
      while (1) {
        string request = get_request(client, nread);
        if (request.empty()){
          close(client);
          return;
        }
        string response = create_response(request, client);
        bool success = send_response(client,response);
        if (!success){
          close(client);
          return;
        }
      }
      send(client, buf, nread, 0);
    }
};