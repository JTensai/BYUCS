#include <iostream>
#include <vector>

#include <semaphore.h>

using namespace std;


class ClientBuffer{
  private:
	vector<int> client_buffer;
    int BUFFER_LIMIT = 100;
	sem_t s,n,e;

  public:
    ClientBuffer(){
        sem_init(&e, 0, BUFFER_LIMIT);
		sem_init(&s, 0, 1);
		sem_init(&n, 0, 0);
    }
    void append(int client){
        sem_wait(&e);
    	sem_wait(&s);
    	client_buffer.push_back(client);
    	sem_post(&s);
    	sem_post(&n);
    }
    int take(){
    	sem_wait(&n);
    	sem_wait(&s);
    	int client = client_buffer[0];
    	client_buffer.erase(client_buffer.begin());
    	sem_post(&s);
        sem_post(&e);
    	return client;
    }
};