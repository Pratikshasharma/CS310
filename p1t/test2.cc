#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

/*A Barrier for a=2 threads (based on midterm16)*/

using namespace std;

int a=2;

void* gate(void* arg) {
  thread_lock(1);

  char *id;
  id = (char *) arg;
  cout <<"gate called with id " << (char *) id << endl;

  a = a -1;
  if(a==0){
      thread_signal(1,1);
   }

  thread_wait(1,2);
  thread_yield();

  thread_unlock(1);
}

void* controller(void* arg){
  thread_lock(1);

  cout << "controller called with arg " << arg << endl;
  thread_create((thread_startfunc_t) gate, (void *) "child thread");
  thread_create((thread_startfunc_t) gate, (void *) "child thread2");
  thread_wait(1,1);
  thread_broadcast(1,2);

  thread_unlock(1);
}

int main() {
  if (thread_libinit( (thread_startfunc_t) controller, (void *) 100)) {
    cout << "thread_libinit failed\n";
    exit(1);
  }
}
