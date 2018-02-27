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

using namespace std;


int maxOrders; // maxOrders on the board 
int NUM_CASHIERS; // total initial number of cashiers 
int recentOrder; // recently completed order by the maker
// int currentOrders=0; // current number of orders in the board
int aliveCashiers=0; // number of alive cashiers 

int globalCashierNum=0;


/* locks */
unsigned int LOCK= 100;
// unsigned int LOCK = 200;
unsigned int CV_BOARD_NOT_FULL = 300;
unsigned int CV_BOARD_FULL = 400;



struct order {
	int cashier;
	int sandwich;
};

std::vector<order> ordersOnBoard;
// std::vector<int> leftSandwichOrders; /* store the sandwich orders left for each cashier -- needed??*/ 
// std::vector<std::deque<int> >totalSandwichList;



void cashier(void* args);
void sandwichMaker(void* args);
void createThreads(void* args);
bool isCashierOnBoard(int cashierNum);

// DEBUG
void printOrdersOnBoard();

char** ORDER_FILES;



// When the fewer than the no of cahsiers- then thats the max ordersw

	void cashier(void* args){
		thread_lock(LOCK);
		int cashierNum = globalCashierNum;
		globalCashierNum++;
		// start_preemptions(true,true,1);
		char* orderFile = (char*) ((void*)args);
		char str[255];
		string line;
		int order_number;

		ifstream fileStream (orderFile);
		if (fileStream.is_open()){
			while(getline(fileStream,line)){
				if(!line.empty()){
					stringstream file_input(line);
					file_input >> order_number;
						// Did not have isCashierOnBoard here 
						while(ordersOnBoard.size() >= maxOrders )
						{
						// wait for board to free space
						// printOrdersOnBoard();
							thread_wait(LOCK,CV_BOARD_NOT_FULL);
						}
						// create an order,
						order myOrder;
						myOrder.sandwich = order_number;
						myOrder.cashier = cashierNum;

						//start_preemptions(true,true,1);
						// add order on board
						ordersOnBoard.push_back(myOrder);

						// currentOrders++;

						cout << "POSTED: cashier " << cashierNum << " sandwich " << myOrder.sandwich << endl;

						// signal new order -- Dont check for if statement 
						// if(currentOrders == maxOrders){
						thread_signal(LOCK,CV_BOARD_FULL);
						// }

						thread_wait(LOCK,myOrder.sandwich);

						// printf("Got Lock %d \n", myOrder.sandwich);
					}
				}

		aliveCashiers--; /* Changed because it did not get decreased properly */
		// printf(" aliveCashiers %d \n ", aliveCashiers);
		thread_signal(LOCK,CV_BOARD_FULL);
		thread_unlock(LOCK);

		}
	}

	void sandwichMaker(void* args){
		// start_preemptions(true,true,1);

		thread_lock(LOCK);
		int lastSandwich = -1;

		for(int i=0; i < NUM_CASHIERS;i++){
			/* create thread for each cashier */
			if(thread_create((thread_startfunc_t) cashier,(void*)ORDER_FILES[i+2])){
				exit(1);
			}
		}

		// check if board has an order
		while(aliveCashiers > 0 || ordersOnBoard.size()>0){
			// Wait for order to be posted if board is not full


			if(aliveCashiers < maxOrders){
				maxOrders = aliveCashiers;
			}

			// printf(" aliveCashiers %d \n ", aliveCashiers);
			// printf(" maxOrders %d \n ", maxOrders);
			// printf(" ordersOnBoard %d \n ", ordersOnBoard.size());

			while(ordersOnBoard.size() < maxOrders){
				// signal the thread to post
				thread_signal(LOCK,CV_BOARD_NOT_FULL);
				
				thread_wait(LOCK,CV_BOARD_FULL);
			}

			// printf(" DONT WAIT \n");

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
			
			// currentOrders--;
			// remove from orders vector
			ordersOnBoard.erase(ordersOnBoard.begin() + index);

			// Order completed
			
			cout << "READY: cashier " << cashierNum << " sandwich " << lastSandwich << endl; 

			thread_signal(LOCK,lastSandwich);

			// USED WAIT HERE 
			// thread_broadcast(LOCK,CV_BOARD_NOT_FULL);
			thread_wait(LOCK,CV_BOARD_FULL);
		}
		
		thread_unlock(LOCK);
		// return; // DID not have it here
	}


	bool isCashierOnBoard(int cashierNum){
		for(int i =0; i < ordersOnBoard.size();i++){
			if(ordersOnBoard[i].cashier == cashierNum){
				return true;
			}
		}
		return false;
	}

	int main (int argc, char* argv[]){
		NUM_CASHIERS = argc - 2;

		if(NUM_CASHIERS < atoi(argv[1])){
			maxOrders = NUM_CASHIERS;
		}else{
			maxOrders = atoi(argv[1]);
		}
		aliveCashiers = NUM_CASHIERS;
		ORDER_FILES = (char **) ((void*)argv);

	  	/* create threads */
	  	if(thread_libinit( (thread_startfunc_t) sandwichMaker,(void*)0)){
			exit(1);

}


	}


	/* creates threads for the sandwich maker and the cashiers */
	// void createThreads(void* args){

		/* create sandwich maker thread */
		// thread_create((thread_startfunc_t) sandwichMaker, (void*) 0 );


		// char str[255];

		

		// NEED a return here ??
	// }


/* Debugging - print the orders on board */
void printOrdersOnBoard(){
	for(int i =0; i < ordersOnBoard.size();i++){
		order myOrder = ordersOnBoard[i];
		printf("Cashier: %d , Sandwich: %d \n", myOrder.cashier,myOrder.sandwich);
	}
}