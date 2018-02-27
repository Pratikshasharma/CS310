#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>


using namespace std;

/* Tests for trying to aquire a lock you already have */

int g=0;

void loop(void *a) {
  char *id;
  int i;

  id = (char *) a;
  cout <<"loop called with id " << (char *) id << endl;
  cout <<"calling thread lock" << endl;
  if(thread_lock(1)==-1){
	cout << "correct failing for requiring same lock" << endl;
  }
  thread_unlock(1);
}

void parent(void *a) {
  int arg;
  arg = (long int) a;

  if (thread_create((thread_startfunc_t) loop, (void *) "child thread")) {
	
	exit(1);
  }
  cout <<"parent should aquire lock first" << endl;
  if(thread_lock(1)==-1){
  	cout << "Bad. Parent should not have failed at this first aquire" << endl;
  }
  loop( (void *) "parent thread");
}

int main() {
  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
	
	exit(1);
  }
}



