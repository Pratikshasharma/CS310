#include "thread.h"
#include "interrupt.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ucontext.h>
#include <deque>
//#include <unordered_map>
#include <iostream>
#include <map>

using namespace std;

/* Defines the relation between lockID and CVID */

typedef struct Monitor 
{
    unsigned int lockID;
    unsigned int cvID; 
};

struct thread_TCB //Individual Threads represented as a struct.
{
	ucontext_t *ucontext_ptr;
	char *stack;
    
};

//DECLARE SHARED VARIABLES WITH STATIC
/*

Use this two to swap context
*/
static thread_TCB *old_context;
static ucontext *new_context;

static std::deque<thread_TCB*> readyQueue ;
static thread_TCB *curr_TCB;//The thread currently running.


static bool HAS_INIT=false;// Boolean to make sure we don't initialize thread library twice


int start(thread_startfunc_t func, void *arg); //The stub Function as described in lecture!


static map< unsigned int, std::deque<thread_TCB*> > allLocks; //A Hashmap of each lockID to lock_queue. The first item of the lock_queue is the lock that has aquired the TCB. 

static map< Monitor*, std::deque<thread_TCB*> > monitors; //A Hashmap of each Monitor / mutex to the cv_queue. 


extern int thread_libinit(thread_startfunc_t func, void *arg){
	//interrupt_disable();
	if (HAS_INIT==true)
	{
   	// interrupt_enable();
    	return -1;

	}
	HAS_INIT=true;

	//Initialize current thread/context to old_context.
	old_context=new thread_TCB;
	old_context->ucontext_ptr=new ucontext_t;

	if (getcontext(old_context->ucontext_ptr)==-1)
	{
    	//interrupt_enable();
    	return -1;
	}
    
	//old_context->DONE=false;
	old_context->stack=new char[STACK_SIZE];
	old_context->ucontext_ptr->uc_stack.ss_sp=old_context->stack;
	old_context->ucontext_ptr->uc_stack.ss_size=STACK_SIZE;
	old_context->ucontext_ptr->uc_stack.ss_flags=0;
	old_context->ucontext_ptr->uc_link=NULL;

	//Declare Parent thread:
	thread_TCB *parent;
	parent = new thread_TCB;// Use new to avoid copying ucontext struct
	parent->ucontext_ptr=new ucontext_t;


	//Parent thread initialization
	int err_get = getcontext(parent->ucontext_ptr);
	if (err_get==-1)
	{
    	//interrupt_enable();
    	return -1;
	}

	//parent->DONE=false;
	parent->stack=new char[STACK_SIZE];
	parent->ucontext_ptr->uc_stack.ss_sp=parent->stack;
	parent->ucontext_ptr->uc_stack.ss_size=STACK_SIZE;
	parent->ucontext_ptr->uc_stack.ss_flags=0;
	parent->ucontext_ptr->uc_link=NULL;

	//modify context pointer
	makecontext(parent->ucontext_ptr, (void (*)()) start, 2, func, arg);

	curr_TCB=parent;
	readyQueue.push_back(parent);//PUSH TO BACK OF READY QUEUE.

	if (swapcontext(old_context->ucontext_ptr, parent->ucontext_ptr)==-1)
	{
   	// interrupt_enable();
    	return -1;
	}

	//DELETE THREADS IF OLD_CONTEXT IS EVER REACTIVATED
	
	// TODO: CLEAN UP AND FREE SPACE.
	delete curr_TCB->stack;
	curr_TCB->ucontext_ptr->uc_stack.ss_sp=NULL;
	curr_TCB->ucontext_ptr->uc_stack.ss_size=0;
	curr_TCB->ucontext_ptr->uc_stack.ss_flags=0;
	delete curr_TCB->ucontext_ptr;
	delete curr_TCB;

	if (readyQueue.empty())

	{

		cout << "Thread library exiting.\n";
		exit(0);

	}
	//Switch to another thread
	curr_TCB=readyQueue.front();

	swapcontext(old_context->ucontext_ptr,curr_TCB->ucontext_ptr);



//interrupt_enable();
//return 0;

}

extern int thread_create(thread_startfunc_t func, void *arg){
    //interrupt_disable();
	if (HAS_INIT==false)
	{
    	//interrupt_enable();
    	return -1;
	}
	//Declare child thread:
	thread_TCB *child;
	child = new thread_TCB;// Use new to avoid copying ucontext struct
	child->ucontext_ptr=new ucontext_t;


	//child thread initialization from curr context
	int err_get = getcontext(child->ucontext_ptr);
	if (err_get==-1)
	{
    	//interrupt_enable();
    	return -1;
	}

	//child->DONE=false;
	child->stack=new char[STACK_SIZE];
	child->ucontext_ptr->uc_stack.ss_sp=child->stack;
	child->ucontext_ptr->uc_stack.ss_size=STACK_SIZE;
	child->ucontext_ptr->uc_stack.ss_flags=0;
	child->ucontext_ptr->uc_link=NULL;



	makecontext(child->ucontext_ptr, (void (*)()) start, 2, func, arg);
    

	//curr_TCB=child;
	readyQueue.push_back(child);//PUSH TO BACK OF READY QUEUE.
	//swapcontext(old_context->ucontext_ptr,child->ucontext_ptr);
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
	if (readyQueue.empty())
	{
   	// interrupt_enable();
    	return 0;//0?
	}


	thread_TCB *new_one=readyQueue.front(); // Pop off parent //disable interruptes
	readyQueue.pop_front(); 
	
	readyQueue.push_back(curr_TCB);  //enable Current thread goes to back
        
        if( readyQueue.empty() )
        { //shouldn't be empty here as we just added current thread.
	return -1;
	}

	//TODO:
	if (swapcontext(curr_TCB->ucontext_ptr,new_one->ucontext_ptr)==-1)
	{
    	//interrupt_enable();
    	return -1;
	}
	 
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
    
	//interrupt_enable();
        curr_TCB=readyQueue.front();
	readyQueue.pop_front();//remove from ready since it now enters run mode.
	func(arg);//Thread has run!
   // interrupt_disable();

    //DELETE THREAD AFTER IT FINISHES RUNNING:WE HAVE TO GO TO ANOTHER CONTEXT TO DO 
    //Go back to old context. Seems to be only context not modified.
    if (swapcontext(curr_TCB->ucontext_ptr,old_context->ucontext_ptr)==-1)
	{
    	//interrupt_enable();
    	return -1;
	}

  //interrupt_enable();

}




/*int thread_wait(unsigned int lockID, unsigned int cvID){
   // Running thread unlocks lockID, Blocks, and moves to the tail of cvID's sleep_queue. RUNNING -> BLOCKED
   // Front of ready_queue runs. READY -> RUNNING
   // Note: When the thread wakes up (due to a later call), and RUNS, it reaquires the lock and returns. Implement this in methods that run. 
   
   //Error: The thread tries to unlock a lock it doesn't have:
   if( allLocks[lockID].front()!=currTLB ) // The front of lockID's queue will be the TLB that currently holds the lock. 
   {
       return -1; //error
   }
   
   return 0;
}*/



int thread_signal(unsigned int lockID, unsigned int cvID){
   // Put head of cvID's sleep_queue on the tail of the ready queue BLOCKED -> READY
   return 0;
}

int thread_broadcast(unsigned int lockID, unsigned int cvID){
   // Put all of the cvID's sleep_queue on the ready queue. BLOCKED -> READY
   return 0;
}




/*Input is lockID*/

//static std::unordered_map<unsigned int, std::deque<thread_TCB*> > allLocks; <---note this is above
int thread_lock(unsigned int lockID){
    interrupt_disable(); 

    // if not in map of locks, create it:
    if (allLocks.find(lockID) == allLocks.end())
       {
              std::deque<thread_TCB*> new_queue;
              allLocks.insert(std::make_pair(lockID, new_queue));
	}

    // Now it is in the map
    std::deque<thread_TCB*> lock_queue = allLocks[lockID];
    
    if( lock_queue.front() == curr_TCB ) //Error: Thread tries to aquire a lock it already has
       {
	     return -1; 
       }

    if ( lock_queue.empty() ) //if(Lock == FREE): curr_thread aquires the lock!
       {
              lock_queue.push_front(curr_TCB); // put curr_thread at the front of lock_queue, curr_TCB aquired the lock!
       }
	
     //Elif the lock is busy then current thread is pushed to the lock_queue. We perform a swap context. 
     else
        { 
          thread_TCB* old_TCB; // store the current thread. *** note is it okay that this is local?***
          old_TCB = curr_TCB;

          curr_TCB = readyQueue.front(); //changes curr_TCB to front of ready list
          readyQueue.pop_front();

	  lock_queue.push_back(old_TCB);//PUSHES RUNNING THREAD TO BACK OF LOCK QUEUE

 	  swapcontext(old_TCB->ucontext_ptr, curr_TCB->ucontext_ptr);  //RUNS FRONT OF READY QUEUE

	}

    interrupt_enable();
    return 0;
}


//Input is lockID - Somewhere need to make a stub so wait() can call unlock
int thread_unlock(unsigned int lockID){
    interrupt_disable();

    if (allLocks.find(lockID) != allLocks.end()) //If the lock is in the map:
    {
        //Error: The thread tries to unlock a lock it doesn't have:
        if( allLocks[lockID].front() != curr_TCB or allLocks[lockID].empty() )
        {
          return -1; //error
        }
        std::deque<thread_TCB*> lock_queue = allLocks[lockID]; // Get lockID from the map. 
	
        allLocks[lockID].pop_front(); // Current thread releases the lock! But keeps running. (aquired lockID == TLB at front of it's lock_queue)
	
        if(!(lock_queue.empty()) ) // IF a thread is waiting on the lock queue: BLOCKED/lQ -> READY QUEUE & AQUIRES LOCK
        {
        
        thread_TCB* t = lock_queue.front(); // Store thread_TCB from the top of lock queue 
        //lock_queue.pop_front(); //nvm DON'T pop this TCB, keep at top of lock queue as t aquired the lock! 
        // Leave lock as aquired by this thread! (Hand off locks from lecture)

        readyQueue.push_back(t); // Put t from lockqueue on back of ready queue  BLOCKED -> READY QUEUE

        }
    }

    // else lockID not in the map, than no lock to unlock-> user error return -1.
    else
    {
        return -1;
    }

    interrupt_enable();
    return 0;
}




