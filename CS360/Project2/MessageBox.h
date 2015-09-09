#include <iostream>
#include <string>
#include <vector>
#include <mutex>

#include <semaphore.h>

using namespace std;


class MessageBox{
  private:
    map<string, vector<pair<string, string>>> message_box;
    mutex message_box_mutex;

  public:
    MessageBox(){
    }


    void store_message(string name, string subject, string message){
      message_box_mutex.lock();
      vector<pair<string, string>> vector_of_messages;
      if (message_box.count(name) > 0){
        vector_of_messages = message_box[name];
      }
      vector_of_messages.push_back(make_pair(subject, message));
      message_box[name] = vector_of_messages;
      message_box_mutex.unlock();
    }

    string create_list_results(string name){
      message_box_mutex.lock();
      string response;
      if (message_box[name].size() > 0){
        stringstream ss1, ss2;
        int i = 0;
        vector<pair<string, string>> vector_of_messages = message_box[name];
        for(pair<string, string> p : vector_of_messages){
          ss1 << ++i << " " << p.first << endl;
        }
        ss2 << "list " << i << "\n" << ss1.str();
        response = ss2.str();
      }
      else{
        response = "list 0\n";
      }
      message_box_mutex.unlock();
      return response;
    }

    string create_read_result(string name, int index){
      message_box_mutex.lock();
      string response;
      // cout << name  << " : " << index << endl;
      // message_box.to_string();
      pair<string, string> message;
      if (message_box[name].size() > 0){
        vector<pair<string, string>> vector_of_messages = message_box[name];
        if (index > vector_of_messages.size() || index <= 0){
          response = "error no such message for that user\n";
        }
        message = vector_of_messages[index-1];

        string subject = message.first;
        string the_message = message.second;
        int length = the_message.size();
        stringstream ss;
        ss << "message " << subject << " " << length << '\n' << the_message;// << endl;
        response = ss.str();
      }
      else{
        response = "error no such message for that user\n";
      }
      message_box_mutex.unlock();
      return response;
    }

    void clear(){
      message_box.clear();
    }

    string to_string(){
      stringstream ss;
      ss << "All Mailbox Contents:" << endl;

      for (auto& entry : message_box){
        ss << entry.first << endl;
        for (auto& entry2 : entry.second){
          ss << "\t" << entry2.first << " : " << entry2.second << endl;
        }
      }
      return ss.str();
    }
};