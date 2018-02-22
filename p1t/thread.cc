#include "interrupt.h" 
#include "thread.h"
#include <stdio.h>  
#include <unistd.h> 
#include <assert.h> 
#include <stdlib.h>
#include <sys/mman.h>

/*Need to edit*/
typedef struct TCB_t{
    ucontext_ptr;
    };

/* Each lock has it's own queue of 'waiting' threads, accessed FIFO */
typedef struct Lock_t{
    unsigned int lockID; 
    int BUSY; /*1 if lock busy, 0 if lock free*/
    std::queue<ucontext_t> lock_queue;
};

/* Each Conditioned Var has it's sleep_queue of blocked/waiting threads on that CV*/
typedef struct CV_t{
    unsigned int cvID;
    std::queue<ucontext_t> sleep_queue; 
};

int thread_libinit(){
    /*Set of Locks, CVs and ready*/
    /*Use ucontext_t or TCBs for threads?*/
    std::set<Lock_t> locks;
    std::set<CV_t> CVs;
    std::queue<ucontext_t> ready_queue;
   
    // Create TCB parent and intialize:
    struct TCB parent;
    
    // Getcontext for parent:
    getcontext(parent->ucontext_ptr);
    char *pstack = new char[STACK_SIZE];
    // Need to make ucontext_ptr
    parent->ucontext_ptr->uc_stack.ss_sp = pstack;
    parent->ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
    parent->ucontext_ptr->uc_stack.ss_flags = 0;
    parent->ucontext_ptr->uc_link = NULL;
    // Direct new thread to the given funct
    makecontext(parent->ucontext_ptr, func);

    // Call the Parent Thread:
    //setcontext();
}

// Input is a func and thread id. 
int thread_create(thread_startfunc_t func, void *arg){
    // Initialize a context structure for child:
    ucontext_t context;
    getcontext(&context);

   // Create TCB child and initialize:
    struct TCB child;
    child.id= arg;
    child.ucontext_ptr = context;

    // Direct new thread to use a different stack:
    char *stack = new char[STACK_SIZE];

    // Need to make ucontext_ptr
    ucontext_ptr->uc_stack.ss_sp = stack;
    ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
    ucontext_ptr->uc_stack.ss_flags = 0;
    ucontext_ptr->uc_link = NULL;
    // Direct new thread to the given funct
    makecontext(&context, func);
    // Put this thread TDB on the ready queue:
    

    // when no runnable threads in the system:
    cout << "Thread library exiting. \n";
    exit(0);
}

int thread_yield(){
   // Put current running thread at end of ready queue
   // Pop top thread off of the ready queue
   // Perform a swap_context();
}

int thread_wait(unsigned int lockID, unsigned int cvID){
   
}

int thread_signal(unsigned int lockID, unsigned int cvID){
}

int thread_broadcast(unsigned int lockID, unsigned int cvID){
}

/*Input is lockID*/
int thread_lock(unsigned int lockID){
    interrupt_disable(); 
    
    //if(value == FREE): value = BUSY;
    //else {... see notes on lock}
     
    interrupt_enable();
}


/*Input is lockID*/
int thread_unlock(unsigned int lockID){
    interrupt_disable();

    // value = FREE;
    // if thread is waiting for lock{ 
    //   t = sleep_queue.pop() 
    //   put t on the ready queue
    //   value = BUSY;}

    interrupt_enable();
}



