#include "thread.h"
#include "interrupt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


using namespace  std;

// Test a thread yielding to itself 
void createThreads(void* args);


int main() {
  if (thread_libinit( (thread_startfunc_t) createThreads, (void *) 0)) {
    cout << "thread_libinit failed" << endl;
    exit(1);
  }
}


void createThreads(void* args){
	cout << thread_lock(1) << endl;
	cout << thread_yield() << endl;
	// cout << thread_unlock(1) << endl;
	if(thread_unlock(1)==-1){
		cout << "thread yielding to itself failed" << endl;
	}
}

