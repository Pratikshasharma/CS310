#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>


using namespace std;



int g=0;

void loop(void *a) {
  char *id;
  int i;

  id = (char *) a;
  cout <<"loop called with id " << (char *) id << endl;
  thread_lock(1);
  thread_unlock(1);
}

void parent(void *a) {
  int arg;
  arg = (long int) a;

  if (thread_create((thread_startfunc_t) loop, (void *) "child thread")) {
	
	exit(1);
  }
    thread_lock(1);

  loop( (void *) "parent thread");
}

int main() {
  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
	
	exit(1);
  }
}



