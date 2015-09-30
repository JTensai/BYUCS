// Taken from Unix Network Programming: The Sockets Networking API, Volume 1,
// Third Edition, by W. Richard Stevens, Bill Fenner, and Andrew M. Rudoff

// Page 700

// This program demonstrates how to use a semaphore to solve the
// synchronization problem.

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

using namespace std;

sem_t lock, holder;

void *create(void *);
void *read(void *);

int counter;
void * empty;

int
main(int argc, char **argv)
{
    pthread_t tidA, tidB;
    
    sem_init(&lock,0,1);
    sem_init(&holder,0,0);

    // create two threads
    pthread_create(&tidA, NULL, &create, &empty);
    pthread_create(&tidB, NULL, &read, &empty);

    // wait for both threads to terminate
    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);

    exit(0);
}

void *
create(void *vptr)
{
    counter = random() % 100;
    cout << "Storing: " << counter << endl;
}

void *
read(void *vptr)
{
    cout << "Reading: " << counter << endl;
}
