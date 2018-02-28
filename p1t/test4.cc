#include "thread.h"
#include "interrupt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include<vector>
#include <deque>

using namespace std;

// Test if a thread unlocks the lock it does not have
// and gets the lock it has
// Test if it acquires a lock already held and releases lock already freed
// Multiple libinit



unsigned int lock1=5;
unsigned int lock2=10;
unsigned int lock3 = 15;

int createThreads();
int thread2();


int main() {
  if (thread_libinit( (thread_startfunc_t) createThreads, (void *) 100)) {
    cout << "thread_libinit failed\n";
    exit(1);
  }
}

int createThreads(){
	thread_lock(lock1); 
	//cout <<thread_lock(lock1)<<endl; //This would also include a test for trying to lock() a lock you already have
	cout<<"Locked lock1" << endl;

  	if (thread_create((thread_startfunc_t) thread2, (void *) "thread 2")) {
  	  cout << "thread_create failed\n";
  	  exit(1);
  	}

	thread_yield();
	if(thread_unlock(lock2)==-1){
	cout << "Did not unlock lock2" << endl;	
	}
	cout<< "Should not unlock lock2" << endl;
	thread_unlock(lock1);
	// Free a lock already freed
	cout<< thread_unlock(lock1) << endl;

	// create another thread to check what happens when you acquire a 
	//lock already locked
	cout << thread_create((thread_start_func_t) thread3, (void*)0)) << endl;
		
	
}

int thread2(){
	thread_lock(lock2);
	cout<<" thread 2 runs with lock2 " <<endl;
	thread_yield();
	thread_unlock(lock2);
}

int thread3(){
	cout << thread_lock(lock3) <<endl;
	cout << thread_lock(lock3) <<endl;
}
