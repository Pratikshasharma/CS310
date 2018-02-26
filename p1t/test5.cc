#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

using namespace std;

int g=0;

void child(void *a) {
if(thread_unlock(1)==-1){

    cout<<"You cannot unlock what you don't have\n";
}
 
 
 
 

 
}

void parent(void *a) {
 

 
  if (thread_create((thread_startfunc_t) child, (void *) "child thread")) {
	cout << "thread_create failed\n";
	exit(1);
  }
	thread_lock(2);
    if(thread_lock(1)==-1){
    cout <<"returns -1\n";
}

thread_unlock(1);
thread_wait(1,5);
thread_unlock(2);

 
}

int main() {
  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
	cout << "thread_libinit failed\n";
	exit(1);
  }
}





