#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

/* Producer - Consumer that ends in Deadlock Test (from midterm16)*/
/* this test uses only 1 condition variable and thus results in a deadlock */

using namespace std;
int sodas;
int capacity = 1;


void* produce(void* arg) {
  thread_lock(1);
 
  char *id;
  id = (char *) arg;
  cout <<"Thread called for" << (char *) id << endl;

  while(sodas==capacity){
      thread_yield();
      thread_wait(1,2);
  }
  sodas += 1;
  thread_signal(1,2);
  thread_yield();

  thread_unlock(1);
}


void* consume(void* arg){
  thread_lock(1);
  
  while(sodas==0){
      thread_yield();
      thread_wait(1,2);
  }
  sodas -= 1;

  thread_signal(1,2);
  thread_yield();

  thread_unlock(1);
}

void* controller(void* arg){
  cout << "Controller thread Called " << endl;
  thread_create((thread_startfunc_t) consume, (void *) "consume1");
  thread_create((thread_startfunc_t) consume, (void *) "consume2");
  thread_create((thread_startfunc_t) produce, (void *) "produce1");
  thread_yield();
  produce((void *) "produce2");
}

int main() {
  if (thread_libinit( (thread_startfunc_t) controller, (void *) 100)) {
    cout << "thread_libinit failed\n";
    exit(1);
  }
}
