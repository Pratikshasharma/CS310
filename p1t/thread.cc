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


/* Each lock has it's own queue of 'waiting' threads, accessed FIFO */// this was isabel
typedef struct Lock_t
{
    unsigned int lockID; //might not be necessary
    int BUSY; /*1 if lock busy, 0 if lock free*/
    std::deque<thread_TCB*> lock_queue;
};


/* Each Conditioned Var has it's sleep_queue of blocked/waiting threads on that CV*/ // this was isabel
typedef struct CV_t
{
    unsigned int cvID;
    std::deque<thread_TCB*> sleep_queue; 
};


//DECLARE SHARED VARIABLES WITH STATIC
/*

Use this two to swap context
*/
static ucontext_t* old_context;
static ucontext* new_context;

static std::deque<thread_TCB*> readyQueue;
static thread_TCB* curr_TCB;//The thread currently running. might not need it


static bool HAS_INIT=false;// Boolean to make sure we don't initialize thread library twice


int start(thread_startfunc_t func, void *arg); //The stub Function as described in lecture!


static std::unordered_map<std::integer,Lock_t*> allLocks; //A Hashmap of each lockID to its Lock_t (which includes queue and status) 
static std::unordered_map<std::integer,CV_t*> allCVs; //A Hashmap of each cvID to its CV_t (which includes each's sleep queue) -- CV_t maybe not necessary

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
	readyQueue.push_back(curr_TCB);//PUSH TO BACK OF READY QUEUE. *****

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

int thread_wait(unsigned int lockID, unsigned int cvID){
   // Running thread unlocks lockID, Blocks, and moves to the tail of cvID's sleep_queue. RUNNING -> BLOCKED
   // Front of ready_queue runs. READY -> RUNNING
   // Note: When the thread wakes up (due to a later call), and RUNS, it reaquires the lock and returns. Implement this in methods that run. 
}

int thread_signal(unsigned int lockID, unsigned int cvID){
   // Put head of cvID's sleep_queue on the tail of the ready queue BLOCKED -> READY
}

int thread_broadcast(unsigned int lockID, unsigned int cvID){
   // Put all of the cvID's sleep_queue on the ready queue. BLOCKED -> READY
}

/*Input is lockID*/
int thread_lock(unsigned int lockID){
    interrupt_disable(); 

    //Note: Lock_t->lockID gives the lockID.  Lock_t->BUSY gives the status. Lock_t->lock_queue gives its queue

    auto search = allLocks.find(lockID); // Map of all locks from <lockIDs : Lock_t>
    if(search!=allLocks.end()) // lockID is found in map
    { 
        //If the lock is free then thread aquires it.
        if(!search->second->BUSY) //->Second gives the value at key=lockID
        { 
            search->second->BUSY=1; // EDIT? Is this correct syntax for Updating a property of a value in allLocks?
        }
	
	//Elif the lock is busy then current thread is pushed to the lock_queue. We perform a swap context.
	else
        { 
          
	  lock_queue.push_back(curr_TCB);//PUSHES RUNNING THREAD TO BACK OF LOCK QUEUE

	  new_context=readyQueue.pop_front(); //Pop off front of ready 

	  //TODO(same as above): figure how to store context pointer in curr_TCB 

 	  swapcontext(old_context,new_context);  //RUNS FRONT OF READY QUEUE

	}

    }

    else //ELSE if lockID is not in the map.
    {
        //Then initialize a new lock, add to the map and aquire it:

        Lock_t *newlock = new Lock_t; //to avoid copying lock_t
        newlock->id = lockID; 
        newlock->status = 1; // SET LOCK TO BUSY
        //newlock->lock_queue =; Is this auto initialized to empty?

        allLocks.insert(std::pair<int,Lock_t*>(lockID, newlock)); //Insert this lockID : newlock into the map.
    }

    interrupt_enable();
}


/*Input is lockID*/
int thread_unlock(unsigned int lockID){
    interrupt_disable();

    // value = FREE;
    // if thread is waiting for lock{ 
    //   t = lock_queue.pop() 
    //   put t on the ready queue
    //   value = BUSY;}

    interrupt_enable();
}



