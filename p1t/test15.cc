#include "thread.h"
#include "interrupt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include<vector>
#include <deque>
#include <sstream>
#include <cstring>

using namespace std;

//Expected output : without preemption
// POSTED: cashier 0 sandwich 53
// POSTED: cashier 1 sandwich 840
// POSTED: cashier 2 sandwich 827
// READY: cashier 0 sandwich 53
// POSTED: cashier 3 sandwich 302
// READY: cashier 3 sandwich 302
// POSTED: cashier 4 sandwich 631
// READY: cashier 4 sandwich 631
// POSTED: cashier 0 sandwich 785
// READY: cashier 0 sandwich 785
// POSTED: cashier 3 sandwich 230
// READY: cashier 2 sandwich 827
// POSTED: cashier 4 sandwich 11
// READY: cashier 1 sandwich 840
// POSTED: cashier 0 sandwich 400
// READY: cashier 0 sandwich 400
// POSTED: cashier 2 sandwich 567
// READY: cashier 2 sandwich 567
// POSTED: cashier 1 sandwich 350
// READY: cashier 1 sandwich 350
// POSTED: cashier 2 sandwich 233
// READY: cashier 2 sandwich 233
// POSTED: cashier 1 sandwich 23
// READY: cashier 3 sandwich 230
// POSTED: cashier 3 sandwich 210
// READY: cashier 3 sandwich 210
// READY: cashier 1 sandwich 23
// READY: cashier 4 sandwich 11
// POSTED: cashier 4 sandwich 5
// READY: cashier 4 sandwich 5
// Thread library exiting.


// With preemption

// POSTED: cashier 0 sandwich 53
// POSTED: cashier 1 sandwich 840
// POSTED: cashier 2 sandwich 827
// READY: cashier 0 sandwich 53
// POSTED: cashier 4 sandwich 631
// READY: cashier 4 sandwich 631
// POSTED: cashier 3 sandwich 302
// READY: cashier 2 sandwich 827
// POSTED: cashier 0 sandwich 785
// READY: cashier 1 sandwich 840
// POSTED: cashier 4 sandwich 11
// READY: cashier 0 sandwich 785
// POSTED: cashier 2 sandwich 567
// READY: cashier 2 sandwich 567
// POSTED: cashier 1 sandwich 350
// READY: cashier 1 sandwich 350
// POSTED: cashier 0 sandwich 400
// READY: cashier 3 sandwich 302
// POSTED: cashier 2 sandwich 233
// READY: cashier 2 sandwich 233
// POSTED: cashier 1 sandwich 23
// READY: cashier 0 sandwich 400
// POSTED: cashier 3 sandwich 230
// READY: cashier 3 sandwich 230
// POSTED: cashier 3 sandwich 210
// READY: cashier 3 sandwich 210
// READY: cashier 1 sandwich 23
// READY: cashier 4 sandwich 11
// POSTED: cashier 4 sandwich 5
// READY: cashier 4 sandwich 5
// Thread library exiting.

int maxOrders; // maxOrders on the board 
int NUM_CASHIERS; // total initial number of cashiers 
int aliveCashiers=0; // number of alive cashiers 

/* locks */
unsigned int LOCK= 100;
unsigned int posted = 500;
unsigned int consumed = 600;
bool preemptions = false;

int postedCount = 0;
int madeCounter = 0;


struct order {
	int cashier;
	int sandwich;
};

struct cashierId{
	int cashier;
};

std::vector<order> ordersOnBoard;


void cashier(void* args);
void sandwichMaker(void* args);
void createThreads(void* args);
bool isCashierOnBoard(int cashierNum);


// DEBUG
void printOrdersOnBoard();

char** ORDER_FILES;

	void cashier(void* args){
		
		// start_preemptions(true,true,1);
		thread_lock(LOCK);


		int cashierNum = (long) args;
		
		// start_preemptions(true,true,1);
		int sandwichList[3];
    	int list0[3] ={53,785,400};
    	int list1[3]= {840, 350,23};
    	int list2[3] = {827, 567,233};
    	int list3[3] = {302, 230,210};
    	int list4[3] = {631, 11,5};

		if(cashierNum==0){
			memcpy(sandwichList, list0, sizeof(sandwichList));
		}else if (cashierNum==1){
			memcpy(sandwichList, list1, sizeof(sandwichList));
		}else if (cashierNum==2){
			memcpy(sandwichList, list2, sizeof(sandwichList));
		}else if (cashierNum==3){
			memcpy(sandwichList, list3, sizeof(sandwichList));
		}else if(cashierNum==4){
			memcpy(sandwichList, list4, sizeof(sandwichList));
		}



		for(int i =0; i < 3; i++){
	
			int order_number = sandwichList[i];
			

			while(ordersOnBoard.size() >= maxOrders){
				thread_wait(LOCK, consumed);
				// printf(" Board Size %d \n ", ordersOnBoard.size());
				// printf(" cashierNum %d \n  ", cashierNum);
			}
			// create an order,
			order myOrder;
			myOrder.sandwich = order_number;
			myOrder.cashier = cashierNum;

			// add order on board
			ordersOnBoard.push_back(myOrder);
			cout << "POSTED: cashier " << cashierNum << " sandwich " << order_number << endl;

			/* New order */
			thread_signal(LOCK,posted);

			/* DEBUG */
			postedCount++;
			
			thread_wait(LOCK,cashierNum);

			
		}
		// thread_lock(1);
		aliveCashiers--; /* Changed because it did not get decreased properly */
		// thread_unlock(1);
		
		maxOrders = std::min(aliveCashiers,maxOrders);
		
		if(aliveCashiers > 0 ){
			thread_signal(LOCK,posted);
		}
		thread_unlock(LOCK);
	}

	void sandwichMaker(void* args){
		if(preemptions){
  			preemptions=false;
  			start_preemptions(true,true, 1);
  			cout<< "Called with preemptions" <<endl;
  		}
	
		thread_lock(LOCK);
		// start_preemptions(true,true,1);

		// Do something for preemptions here
		for(int i=0; i < NUM_CASHIERS;i++){
			thread_create((thread_startfunc_t) cashier,(void*)i);
		}

		int lastSandwich = -1;

		// check if board has an order
		while(aliveCashiers > 0 ){


			while(ordersOnBoard.size() < maxOrders){
				thread_wait(LOCK,posted);
			}

			
			// get the index of next sandwich 
			int minimumDiff = 1000; /* 999 +1 */
			int index;
			for(int i=0; i < ordersOnBoard.size();i++){
				order myOrder = ordersOnBoard[i];
				if(std::abs(myOrder.sandwich - lastSandwich) < minimumDiff){
					minimumDiff = std::abs(myOrder.sandwich - lastSandwich);
					index = i;
				}
			}

			// update lastSandwich
			lastSandwich = ordersOnBoard[index].sandwich;
			int cashierNum = ordersOnBoard[index].cashier;
			
			// remove from orders vector
			ordersOnBoard.erase(ordersOnBoard.begin() + index);

			// Order completed
			
			/* DEBUG */
			madeCounter++;

			cout << "READY: cashier " << cashierNum << " sandwich " << lastSandwich << endl; 

			thread_signal(LOCK,consumed);
			
			thread_signal(LOCK,cashierNum);

			
		}
		
		thread_unlock(LOCK);
		// return; // DID not have it here
	}



	int main (){
		NUM_CASHIERS = 5;

		aliveCashiers = NUM_CASHIERS;
		maxOrders = 3;
		// ORDER_FILES = (char **) ((void*)argv);

	  	/* create threads */
	  	if(thread_libinit( (thread_startfunc_t) sandwichMaker,(void*)0)){
			exit(1);
	}

}
