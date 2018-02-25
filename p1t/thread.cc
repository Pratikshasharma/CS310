//g++ -o deli thread.o deli.cc libinterrupt.a -ldl

#include "thread.h"
// #include "interrupt.h"
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ucontext.h>
#include <deque>

using namespace std;

 ucontext_t *ucontext_ptr;
   
    

//DECLARE SHARED VARIABLES WITH STATIC
/*

Use this two to swap context
*/
static ucontext_t *old_context;


static std::deque<ucontext_t*> readyQueue ;
static ucontext_t *curr=new ucontext_t;//The thread currently running.



static bool HAS_INIT=false;// Boolean to make sure we don't initialize thread library twice


int start(thread_startfunc_t func, void *arg); //The stub Function as described in lecture!


extern int thread_libinit(thread_startfunc_t func, void *arg){
	//interrupt_disable();
	if (HAS_INIT==true)
	{
   	// interrupt_enable();
    	return -1;

	}
	HAS_INIT=true;

	//Initialize current thread/context to old_context.
    
	old_context=new ucontext_t;

	if (getcontext(old_context)==-1)
	{
    	//interrupt_enable();
    	return -1;
	}
    
	//old_context->DONE=false;
	char *stack=new char[STACK_SIZE];
	old_context->uc_stack.ss_sp=stack;
	old_context->uc_stack.ss_size=STACK_SIZE;
	old_context->uc_stack.ss_flags=0;
	old_context->uc_link=NULL;

	//Declare Parent thread:
	ucontext_t *parent=new ucontext_t;
   

	//Parent thread initialization
	int err_get = getcontext(parent);
	if (err_get==-1)
	{
    	//interrupt_enable();
    	return -1;
	}

	//parent->DONE=false;
	char *p_stack=new char[STACK_SIZE];
	parent->uc_stack.ss_sp=p_stack;
	parent->uc_stack.ss_size=STACK_SIZE;
	parent->uc_stack.ss_flags=0;
	parent->uc_link=NULL;

	//modify context pointer
	makecontext(parent, (void (*)()) start, 2, func, arg);

	curr=old_context;

	if (swapcontext(old_context, parent)==-1)
	{
   	// interrupt_enable();
    	return -1;
	}

	//DELETE THREADS IF OLD_CONTEXT IS EVER REACTIVATED
    
	// TODO: CLEAN UP AND FREE SPACE.
    
   while(!(readyQueue.empty())){
    
	delete curr->uc_stack.ss_sp;
	curr->uc_stack.ss_sp=NULL;
	curr->uc_stack.ss_size=0;
	curr->uc_stack.ss_flags=0;
	delete curr;
    
	curr=readyQueue.front();
	readyQueue.pop_front();
	cout << "I did reached cleanup ON WHILE LOOP END\n";

    
	//TODO:Switch to another thread
   if (swapcontext(old_context,curr)==-1)
	{
    
    	//interrupt_enable();
    	return -1;
	}
    

}

    
    	cout << "Thread library exiting.\n";
    	exit(0);

    

}

extern int thread_create(thread_startfunc_t func, void *arg){
	//interrupt_disable();
	if (HAS_INIT==false)

	{
    	//interrupt_enable();
    
    	return -1;
	}
    
	//Declare child thread:
	ucontext_t *child=new ucontext_t;
    
	//child thread initialization from curr context
	int err_get = getcontext(child);
	if (err_get==-1)
	{
    
    	//interrupt_enable();
    	return -1;
	}

	//child->DONE=false;
	char *stack=new char[STACK_SIZE];
	child->uc_stack.ss_sp=stack;
	child->uc_stack.ss_size=STACK_SIZE;
	child->uc_stack.ss_flags=0;
	child->uc_link=NULL;



	makecontext(child, (void (*)()) start, 2, func, arg);
    

	readyQueue.push_back(child);//PUSH TO BACK OF READY QUEUE.
	//interrupt_enable();


	return 0;

}

extern int thread_yield(void){
   // interrupt_disable();
	if (HAS_INIT==false)
	{
    	//interrupt_enable();
    	return -1;
	}
    
    

    //cout <<curr<<'\n';
    //cout <<readyQueue.front()<<'\n';

    
    if(readyQueue.empty()){
    return 0;

}
   
    
    ucontext_t *oldOne;
    oldOne=curr;

    curr=readyQueue.front();
    readyQueue.pop_front();
    
   	 readyQueue.push_back(oldOne);
    //cout << "Size is"<<readyQueue.size()<<'\n';
    
    
	//TODO:
	if (swapcontext(oldOne,curr)==-1)
	{
    	//interrupt_enable();
    
    	return -1;
	}
   	// interrupt_enable();
  	 
    
    
    
    
	 
 	//interrupt_enable();
 	return 0;

}
//Stub Function
int start(thread_startfunc_t func,void *arg){
    

   // interrupt_disable();
	if (HAS_INIT==false)
	{
    	//interrupt_enable();
    	return -1;
	}

	 
    
	func(arg);

	//DELETE THREAD AFTER IT FINISHES RUNNING:WE HAVE TO GO TO ANOTHER CONTEXT TO DO SO.
	//Go back to old context. Seems to be only context not modified.
	if (swapcontext(curr,old_context)==-1)
	{
    	//interrupt_enable();
    
    	return -1;
	}

  //interrupt_enable();
    return 0;//Thread exits

}













