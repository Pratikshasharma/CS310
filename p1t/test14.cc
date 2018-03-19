#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>
#include<stdio.h>

using namespace std;
// For H , a thread exits because 
// test that a thread exits after another thread with the lock exits 
// Signal without the lock 

// lock yield, signal, wait signal

void secondInit();
void* createThread(void* args);
void createSecondThread(void* args);

void createTwoThreads();

int main (){
	cout<<thread_libinit((thread_startfunc_t) createThread,NULL)<< endl;
	return 0;
}


void* createThread(void* args){
	
	cout << thread_create((thread_startfunc_t) createTwoThreads, NULL) << endl;
	
	printf(" Initial thread has the lock so runs again \n ");
	// Exits the thread , second thread is stuck 
}

void createTwoThreads(){
	if(thread_create((thread_startfunc_t) createSecondThread, NULL) == -1){
		printf(" Error in thread creation \n ");
	}

	if(thread_create((thread_startfunc_t) createSecondThread, NULL) == -1){
		printf(" Error in thread creation \n ");
	}
}

void createSecondThread(void* args ){
	cout << thread_lock(2) << endl;

	cout << thread_yield() << endl;
	
	cout << thread_signal(1,3) << endl;

	cout << thread_wait(1,3) << endl;

	printf(" signal here \n ");
	cout << thread_signal(1,3) << endl;
}