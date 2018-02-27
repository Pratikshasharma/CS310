#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>

using namespace std;

/* Based on Midterm 2016 Highway Question - this version has no explicit 'errors' but is sorta complicated so i feel like a buggy thread library may mess it up. But I'll also make an errory version for testing*/

int afree = 1;//true
int dir = 0;
int waiters[2] = {0,0};
int cars = 0;
void parent(void *a);

int main() {

  if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
	cout << "thread_libinit failed\n";
	exit(1);
  }
}

// myway = 1?
void Arrive(int myway) {
 thread_lock(1);
 cout << "CALLS ARRIVE" << myway << endl;

 int otherway;
 if(myway) {otherway=1;}
 else{otherway=0;}

 while( (!afree && (dir != myway)) or (waiters[otherway] > 0) ){
    waiters[myway]+=1;
    cout << "Waits?" << cars << endl;
    thread_wait(1,2); //right cv?
  }
  afree = 0;//false
  dir = myway;
  cars+=1;

 thread_unlock(1);
}

void Depart(int myway) {
 thread_lock(1);

 cout << "CALLS DEPART" << myway << endl;
 int otherway;
 if(myway) {otherway=1;}
 else{otherway=0;}

 cars-=1;
 if(cars == 0){
    afree = 1;//true
    waiters[myway] = 0;
    waiters[otherway] = 0;

    cout << "Broadcasts?" << cars << endl;

    thread_broadcast(1,1); //write cv?
 } 
 thread_unlock(1);
}


void parent( void *a){
  int ar = 1;
  int dr = 0;
  cout << "CALLS PARENT" << endl;

  if (thread_create((thread_startfunc_t) Arrive, (void*) ar)) {
	cout << "thread_create1 failed\n";
	exit(1);
  }
  if (thread_create((thread_startfunc_t) Depart, (void*) dr )) {
	cout << "thread_create2 failed\n";
	exit(1);
  }
   cout << "threads created!" << endl;
  thread_yield();
}






