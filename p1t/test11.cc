#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

using namespace std;

int g=0;

void friend1(void *a) {

  char *id;
  int i;
  thread_lock(20);
  id = (char *) a;
  cout <<"friend called with id " << (char *) id << endl;

  for (i=0; i<5; i++, g++) {
    cout << id << ":\t" << i << "\t" << g << endl;
    if (thread_yield()) { // if thread_yield() not equal to
      cout << "thread_yield failed\n";
      exit(1);
    }
  }
   thread_wait(20,1);
   thread_unlock(20);

}


void friend2(void *a) {

  char *id;
  int i;

  id = (char *) a;
  
  thread_lock(2);

  if (thread_yield()) { // if thread_yield() not equal to
      cout << "thread_yield failed\n";
      exit(1);
  }
  thread_signal(20,1);
  thread_unlock(2);

}

void friend3(void *a) {

  char *id;
  int i;
  thread_lock(1);
  thread_lock(3);
  thread_lock(11);
  id = (char *) a;
  cout <<"friend called with id " << (char *) id << endl;
  for (i=0; i<7; i++, g++) {
    cout << id << ":\t" << i << "\t" << g << endl;
    if (thread_yield()) { // if thread_yield() not equal to
      cout << "thread_yield failed\n";
      exit(1);
    }
  }
  thread_unlock(1);
  thread_unlock(3);
  thread_unlock(11);

}

void parent(void *a) {
  int arg;
  arg = (long int) a;

  cout << "parent called with arg " << arg << endl;
  if (thread_create((thread_startfunc_t) friend1, (void *) "1 thread")) {
    cout << "thread_create failed\n";
    exit(1);
  }
  if (thread_create((thread_startfunc_t) friend2, (void *) "2 thread")) {
    cout << "thread_create failed\n";
    exit(1);
  }
  if (thread_create((thread_startfunc_t) friend3, (void *) "3 thread")) {
    cout << "thread_create failed\n";
    exit(1);
  }

  friend1( (void *) "parent thread");
}

int main() {
  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
    cout << "thread_libinit failed\n";
    exit(1);
  }
}
