#include "thread.h"
#include "interrupt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


using namespace  std;

// Test a thread yielding to itself 
void createThreads(void* args);
void createChildThreads(void* args);
unsigned int lock = 2;

int global = 0;



int main() {
  if (thread_libinit( (thread_startfunc_t) createThreads, (void *) 0)) {
    cout << "thread_libinit failed" << endl;
    exit(1);
  }
}


void createThreads(void* args){
	cout << thread_lock(lock) << endl;
	cout << thread_create((thread_startfunc_t) createChildThreads,(void*)1) << endl;

	cout << thread_create((thread_startfunc_t) createChildThreads,(void*)2) << endl;
	
	
	cout << thread_broadcast(lock,1) << endl;

	cout << thread_unlock(lock) << endl;

	cout << thread_yield() << endl;

}

void createChildThreads(void* args){
	cout << "child thread " << endl;
	cout << ((long) args)<< endl;
	
	thread_lock(lock);
	global = global + (long) args ;
	cout << global << endl;

	while (global >= 3){
		thread_wait(lock,1);
	}
	
	cout << global << endl;
	cout << thread_unlock(lock) << endl;
}

