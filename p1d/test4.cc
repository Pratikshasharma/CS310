
#include "thread.h"
#include "interrupt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


using namespace std;

// Test if a thread unlocks the lock it does not have
// and gets the lock it has

unsigned int lock1=5;
unsigned int lock2=10;

int createThreads();
int thread2();



int main(){
	// thread_create(createThreads,NULL);
	thread_create((thread_startfunc_t)createThreads,NULL);
	exit(0);
}

int createThreads(){
	thread_lock(lock1);
	std::cout << thread_lock(lock1) << endl;
	std::cout << "Locked lock1" << endl;
	// thread_create( thread2,NULL);
	thread_create( (thread_startfunc_t) thread2,NULL);
	thread_yield();
	thread_unlock(lock2);
	std::cout << "Should not unlock lock2" << endl;
	thread_unlock(lock1);
}

int thread2(){
	thread_lock(lock2);
	std::cout << "thread 2 runs with lock2 " << endl;
	thread_yield();
	thread_unlock(lock2);
}