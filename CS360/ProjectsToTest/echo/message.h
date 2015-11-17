#pragma once

#include <string>

using namespace std;

class Message {
public:
    Message(string, string);
    ~Message();
    string getSubject();
    void setSubject(string);
    string getMessage();
    void setMessage(string);
    
private:
   
   string subject;
   string message;
};
