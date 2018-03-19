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
 // cout <<"loop called with id " << (char *) id << endl;

  for (i=0; i<5; i++, g++) {
    //cout << id << ":\t" << i << "\t" << g << endl;
    if (thread_yield()) {
      cout << "thread_yield failed\n";
      exit(1);
    }
  }
}

void parent(void *a) {
  int arg;
  arg = (long int) a;

  //cout << "parent called with arg " << arg << endl;
for(int i=0;i<10;i++){
  if (thread_create((thread_startfunc_t) loop, (void *) "child thread")) {
    cout << "thread_create failed\n";
    exit(1);
  }
}
  //for( int=0  //So only 10 threads active at a time- but toooons of threads are created over the course of the program.
  for(int i=0;i<9000000;i++)
   {	
	loop( (void *) "parent thread");
   }
  cout << "done?" << endl;
}

int main() {
  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
    cout << "thread_libinit failed\n";
    exit(1);
  }
}

