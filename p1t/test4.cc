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

/*RETURNS ERROR - EDIT HERE */

unsigned int lock1=5;
unsigned int lock2=10;

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
}

int thread2(){
	thread_lock(lock2);
	cout<<" thread 2 runs with lock2 " <<endl;
	thread_yield();
	thread_unlock(lock2);
}
