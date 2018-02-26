#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

/* TEST CASE : Calling libinit before other functions */
using namespace std;

void loop(void *a) {
  //cout <<"loop called with id " << (char *) id << endl;
}

void parent(void *a){
  int arg;
  arg = (long int) a;

  cout << "parent called with arg " << arg << endl;

  loop( (void *) "parent thread");

}

int main() {
  if (thread_create((thread_startfunc_t) loop, (void *) "child thread")) {
    cout << "thread_create failed\n";
    exit(1);
  }

  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
    cout << "thread_libinit failed\n";
    exit(1);
  }
}
