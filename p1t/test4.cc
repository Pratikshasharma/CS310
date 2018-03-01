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
int thread3();


int main() {
  if (thread_libinit( (thread_startfunc_t) createThreads, (void *) 100)) {
    cout << "thread_libinit failed" << endl;
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

	if(thread_yield()){
		cout << "thread yield failed" << endl;
	}

	if(thread_unlock(lock2)==-1){
		cout << "correct: unlock lock2 should fail " << endl;	
	}
	
	if(thread_unlock(lock1)!=0){
		cout << "failed: unlock lock1 " << endl;
	} 
	
	// Free a lock already freed
	if(thread_unlock(lock1)!=-1){
		cout << "failed: freeing an already freed lock " << endl;
	} 

	// create another thread to check what happens when you acquire a 
	//lock already locked
	cout << thread_create((thread_startfunc_t) thread3, (void*)0) << endl;
		
	
}

int thread2(){
	thread_lock(lock2);
	cout<<" thread 2 runs with lock2 " <<endl;
	thread_yield();
	thread_unlock(lock2);
}

int thread3(){
	cout << thread_lock(lock3) <<endl;
	if(thread_lock(lock3) !=-1){
		cout << "failed: locking an already locked lock " <<endl;
	}
	
}
