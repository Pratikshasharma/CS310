#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

using namespace std;

int g=0;

void friend1(void *a) {

  char *id;
  int i;

  id = (char *) a;
  cout <<"friend called with id " << (char *) id << endl;

  for (i=0; i<5; i++, g++) {
    cout << id << ":\t" << i << "\t" << g << endl;
    if (thread_yield()) { // if thread_yield() not equal to
      cout << "thread_yield failed\n";
      exit(1);
    }
  }

}


void friend2(void *a) {
  thread_lock(2);
  char *id;
  int i;
  
  id = (char *) a;

  thread_wait(2,2); // should got to thread 3 

  cout <<"friend called with id " << (char *) id << endl;
  
  if (thread_yield()) { // if thread_yield() not equal to
      cout << "thread_yield failed\n";
      exit(1);
  }
  
  thread_unlock(2);

}

void friend3(void *a) {

  char *id;
  int i;

  id = (char *) a;
  cout <<"friend called with id " << (char *) id << endl;
  thread_wait(2,2);
  for (i=0; i<5; i++, g++) {
    cout << id << ":\t" << i << "\t" << g << endl;
    if (thread_yield()) { // if thread_yield() not equal to
      cout << "thread_yield failed\n";
      exit(1);
    }
    if(g%2==0){
	if(thread_yield()){
	exit(1);
         }
    }
  }

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
  thread_broadcast(2,2);
}

int main() {
  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
    cout << "thread_libinit failed\n";
    exit(1);
  }
}
