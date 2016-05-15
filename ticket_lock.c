// non-critical section
// mytick ← fetch-and-add(ticket, 1)
// await mytick = serving
// critical section
// serving ← mytick+1

// fetch-and-add(common, local, x) is
// local ← common
// common ← common + x
// Requirements
// a. Create a function to implement the fetch-and-add or fetch-and-increment
// operation on which ticket locks are built.
// You should plan to use the pthread mutex locks to protect the increment
// or add operation.
// b. Create a function to aquire (lock) a ticket lock.
// c. Create a function to release (unlock) a ticket lock.
//
// d. Use the aquire and release calls around the critical section of your counter loop.
//    As with the first assignment, lock and unlock for each count.
//    I.e. the lock and unlock should be inside the loop structure.
//

// mytick = nextTick; //mytick ← fetch-and-add(ticket, 1)
// nextTick = mytick + 1;
#include <stdio.h>
#include <pthread.h>
#include <sched.h>

#define MAX_COUNT 500000

pthread_t tid[2];
pthread_mutex_t lock;
volatile int x = 0;

int myTicket = 0;
int nextTicket = 1;
int serving = 1;


void fetch_and_add() {
  myTicket = nextTicket;
  nextTicket = myTicket + 1;
}

void * criticalSection() {
  x++;
}

void* aquireLock() {
  pthread_mutex_lock(&lock);
  fetch_and_add();
  while(myTicket != serving);
}

void* release() {
  serving++;
  pthread_mutex_unlock(&lock);
}

void* ticket_lock() {
  int i = 0;
  for(i; i < MAX_COUNT; i++) {
    aquireLock();
    criticalSection();
    release();
  }
}

int main() {
  pthread_attr_t myattr;
  cpu_set_t cpuset;

  pthread_create(&(tid[0]), NULL, &ticket_lock, NULL);
  pthread_create(&(tid[1]), NULL, &ticket_lock, NULL);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_mutex_destroy(&lock);

  printf("Final output of x should be %d\n", MAX_COUNT*2);
  printf("x: %i\n", x);
}
