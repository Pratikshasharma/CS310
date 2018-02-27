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

/*Nevermind, probably delete this as its not an error */
// Testing that the library returns0 when you try to yield with no runnable thread. 

int thread();

int thread(){
  //Tries to yield, but there is nothing on the ready queue so should return error. 
  cout << "Thread tries to yield" << endl;
  if(thread_yield()==-1){
   	cout << "Thread did not yield (correct behavior for test!) " << endl;
	return -1;	
   }
  cout << "Does the thread keep going on" << endl;
  thread_lock(1);
}

int main() {
  if (thread_libinit( (thread_startfunc_t) thread, NULL)) {
    cout << "thread_libinit failed\n";
    exit(1);
  }
}
