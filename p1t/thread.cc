#include "thread.h"
#include "interrupt.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ucontext.h>
#include <deque>

struct thread_TCB //Individual Threads represented as a struct.
{
	ucontext_t *ucontext_ptr;
	char *stack;
	bool DONE;// set to true when thread is Dead!
};

//DECLARE SHARED VARIABLES WITH STATIC
/*

Use this two to swap context
*/
static ucontext_t* old_context;
static ucontext* new_context;

static std::deque<thread_TCB*> readyQueue ;
static thread_TCB* curr_TCB;//The thread currently running. might not need it


static bool HAS_INIT=false;// Boolean to make sure we don't initialize thread library twice


int start(thread_startfunc_t func, void *arg); //The stub Function as described in lecture!


extern int thread_libinit(thread_startfunc_t func, void *arg){
	if (HAS_INIT==true)
	{
		return -1;

	}
	HAS_INIT=true;
	//Initialize current context to old_context.
	old_context=new ucontext_t;
	getcontext(old_context);

	old_context->uc_stack.ss_sp=new char[STACK_SIZE];
	old_context->uc_stack.ss_size=STACK_SIZE;
	old_context->uc_stack.ss_flags=0;
	old_context->uc_link=NULL;

	//Declare Parent thread:
	thread_TCB *parent;
	parent = new thread_TCB;// Use new to avoid copying ucontext struct
	parent->ucontext_ptr=new ucontext_t;


	//Parent thread initialization
	int err_get = getcontext(parent->ucontext_ptr);
	if (err_get==-1)
	{
		return -1;
	}

	parent->DONE=false;
	parent->stack=new char[STACK_SIZE];
	parent->ucontext_ptr->uc_stack.ss_sp=parent->stack;
	parent->ucontext_ptr->uc_stack.ss.size=STACK_SIZE;
	parent->ucontext_ptr->uc_stack.ss_flags=0;
	parent->ucontext_ptr->uc_link=NULL;

	

	makecontext(parent->ucontext_ptr, (void (*)()) start, 2, func, arg);

	curr_TCB=parent;
	readyQueue.push_back(curr_TCB);//PUSH TO BACK OF READY QUEUE.

	swapcontext(old_context, parent->ucontext_ptr);



}

extern int thread_create(thread_startfunc_t func, void *arg){
	if (HAS_INIT==false)
	{
		return 0;
	}
	//Declare child thread:
	thread_TCB *child;
	child = new thread_TCB;// Use new to avoid copying ucontext struct
	child->ucontext_ptr=new ucontext_t;


	//Parent thread initialization from curr context
	int err_get = getcontext(child->ucontext_ptr);
	if (err_get==-1)
	{
		return -1;
	}

	child->DONE=false;
	child->stack=new char[STACK_SIZE];
	child->ucontext_ptr->uc_stack.ss_sp=parent->stack;
	child->ucontext_ptr->uc_stack.ss.size=STACK_SIZE;
	child->ucontext_ptr->uc_stack.ss_flags=0;
	child->ucontext_ptr->uc_link=NULL;



	makecontext(child->ucontext_ptr, (void (*)()) start, 2, func, arg);

	
	swapcontext(old_context,child->ucontext_ptr);



}

extern int thread_yield(void){
	if (HAS_INIT==false)
	{
		return -1;
	}

	new_context=readyQueue.pop_front();
	//TODO: figure how to store context pointer in curr_TCB 


 	swapcontext(old_context,new_context);


}
//Stub Function
int start(thread_startfunc_t func,void *arg){

	if (HAS_INIT==false)
	{
		return -1;
	}

	

	func(arg);
	if (readyQueue.empty())
	{
		exit(0);//No thread to switch to, thus exit!
	}

	
	//Remove from ready queue since its about to run.
	new_context=readyQueue.pop_front();

	//switch context.
	swapcontext(old_context,new_context);

	// TODO

	//Clean up here and remove stack. Will come and clean up





}