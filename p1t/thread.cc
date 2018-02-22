#include "interrupt.h" // interrupt_disable(), interrupt_enable()   use for atomicity
#include "thread.h"

#include <stdio.h>  // Not sure if we need this one (used for size_t in p0)
#include <unistd.h> // needed for sbrk
#include <assert.h> // needed for asserts
#include <stdlib.h>
#include <sys/mman.h>


typedef struct TCB{
    // Need to add to this:
    ucontext_ptr;
    }tcb_t;

int thread_libinit(){
    /* A thread is always on one of these three queues, or running */
    HashSet<int> lock_queue = new HashSet<int>();
    //Do we put in ucontext's or TCB's for ready_queue?
    std::queue<ucontext_t> sleep_queue;
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


/*Input is lockID*/
int thread_lock(unsigned int lock){
    interrupt_disable(); 
    
    //if(value == FREE): value = BUSY;
    //else {... see notes on lock}
     
    interrupt_enable();
}

/*Input is lockID*/
int thread_unlock(unsigned int lock){
    interrupt_disable();

    // value = FREE;
    // if thread is waiting for lock{ 
    //   t = sleep_queue.pop() 
    //   put t on the ready queue
    //   value = BUSY;}

    interrupt_enable();
}


