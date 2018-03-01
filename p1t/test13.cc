#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <assert.h>
#include<stdio.h>

using namespace std;
// Calling thread libinit twice 

void secondInit();
void* createThread(void* args);


int main (){
	cout<<thread_libinit((thread_startfunc_t) createThread,NULL)<< endl;
	return 0;
}


void* createThread(void* args){
	cout<< "Creating another thread libinit" << endl;
	if(thread_libinit( (thread_startfunc_t) secondInit,NULL) == -1){
		printf(" Cannot call libinit twice. \n ");
	}
	
}


void secondInit(){
	cout<< thread_lock(1)<<endl;
}