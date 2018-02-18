#include "thread.h"
#include "interrupt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include<vector>

using namespace std;


int maxOrders; // maxOrders on the board 
int NUM_CASHIERS; // total initial number of cashiers 
int recentOrder; // recently completed order by the maker
int currentOrders=0; // current number of orders in the board
int aliveCashiers=0; // number of alive cashiers 


/* locks */
int SANDWICHMAKER_LOCK= 100;
int CASHIER_LOCK = 200;
int CV_BOARD_NOT_FULL = 300;
int CV_BOARD_FULL = 400;




struct order {
	int cashier;
	int sandwich;
};

std::vector<order> ordersOnBoard;
std::vector<std::vector<int> >totalSandwichList;


void cashier(void* args);
void sandwichMaker(void* args);
void createThreads(void* args);

char** ORDER_FILES;



// When the fewer than the no of cahsiers- then thats the max ordersw

	void cashier(void* args){
		/* lock the thread first */
		// printf(" cashier \n");

		int cashierNum = (long int) args;
		std::vector<int> sandwiches = totalSandwichList[cashierNum];

		/* Check if the cashier is alive */
		while(sandwiches.size()>0){

			// printf("Sandwich Size >0 : %d \n", sandwiches.size());

			/* lock the thread */
			thread_lock(CASHIER_LOCK);
			/*  if the board is full */
			while(currentOrders >= maxOrders){
				// wait for board to free space
				thread_wait(CASHIER_LOCK,CV_BOARD_NOT_FULL);
			}
			// push order into the board
			// if(sandwiches.size() >0){
				// create an order
				order myOrder;
				myOrder.sandwich = sandwiches.front();
				myOrder.cashier = cashierNum;

				// Empty the sandwiches vector
				sandwiches.pop_back();
				// printf("sandwich size after: %d \n ", sandwiches.size());

				// add order on board 
				ordersOnBoard.push_back(myOrder);
				// printf("board size after: %d \n ", ordersOnBoard.size());
				
				currentOrders++;
				// printf("Current orders: %d \n ", currentOrders);

				cout << "POSTED: cashier " << cashierNum << " sandwich " << myOrder.sandwich << endl;

				// signal new order
				if(currentOrders == maxOrders){
					thread_signal(SANDWICHMAKER_LOCK,CV_BOARD_FULL);
				}
				// unlock the sandwichmaker thread
				// thread_unlock(SANDWICHMAKER_LOCK);
				// Wait for order to be completed

				// wait for your sandwich to be made 
				thread_wait(CASHIER_LOCK,myOrder.sandwich);

				cout << "READY: cashier " << cashierNum << " sandwich " << myOrder.sandwich << endl;

				// signal maker  -- PPOTENTIAL BUG- FIX IT 
				thread_signal(SANDWICHMAKER_LOCK,CV_BOARD_FULL);

				// wait for signal to post next order
				// thread_wait(CASHIER_LOCK,CV_BOARD_NOT_FULL);

			// }
		}

		aliveCashiers--;
		// signal maker order has been posted
		if(aliveCashiers < maxOrders && aliveCashiers > 0){
			// adjust maxOrders
			maxOrders--;
			thread_signal(SANDWICHMAKER_LOCK,CV_BOARD_FULL);
		}
		// unlock thread from cashier
		thread_unlock(CASHIER_LOCK);
		return;
	}


	void sandwichMaker(void* args){
		printf(" maker \n ");
		
		int lastSandwich = -1;

		// check if board has an order
		while(aliveCashiers > 0 ){
			thread_lock(SANDWICHMAKER_LOCK);

			// Wait for order to be posted if board is not full
			while(ordersOnBoard.size() < maxOrders){

				// signal the thread to post
				thread_signal(CASHIER_LOCK,CV_BOARD_NOT_FULL);
				thread_wait(SANDWICHMAKER_LOCK,CV_BOARD_FULL);
			}

			// unlock if no more alive cashiers
			if(aliveCashiers ==0){
				thread_unlock(SANDWICHMAKER_LOCK);
			}

			// get the index of next sandwich 
			int minimumDiff = 999;
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
			
			// reduce currentOrders
			currentOrders--;
			// remove from orders vector
			ordersOnBoard.erase(ordersOnBoard.begin() + index);
			// signal the cashier
			thread_signal(CASHIER_LOCK,lastSandwich);

			// thread_wait(SANDWICHMAKER_LOCK,CV_BOARD_FULL); // wait to print
			// broadcast order can be posted
			thread_broadcast(CASHIER_LOCK,CV_BOARD_NOT_FULL);

		}
		// 
		thread_unlock(SANDWICHMAKER_LOCK);
		return;

	}



	int main (int argc, char* argv[]){
		NUM_CASHIERS = argc - 2;

		if(NUM_CASHIERS < atoi(argv[1])){
			maxOrders = NUM_CASHIERS;
		}else{
			maxOrders = atoi(argv[1]);
		}
		aliveCashiers = NUM_CASHIERS;
		// ORDER_FILES = argv;
		ORDER_FILES = (char **) ((void*)argv);
		
		// cout << "The current files are:" << '\n ';
	 	//  	for(int i = 0; i < NUM_CASHIERS;i++){
	 	//  		std::cout << ' ' << ORDER_FILES[i+2];
	 	//  		cout << '\n';
	 	//  	}

	  	/* create threads */
	  	thread_libinit( (thread_startfunc_t) createThreads,(void*)0);

	}


	/* creates threads for the sandwich maker and the cashiers */
	void createThreads(void* args){
		printf(" CREATE THREADS \n ");

		/* create sandwich maker thread */
		thread_create((thread_startfunc_t) sandwichMaker, (void*) 0 );

		char str[255];

		for(int i=0; i < NUM_CASHIERS;i++){
			char* orderFile = (char*) ((void*)ORDER_FILES[i+2]);
			ifstream fileStream(orderFile);

			std:: vector<int> sandwiches ;
			while(!fileStream.eof()){
				fileStream.getline(str,255);
				if(!fileStream.eof() && !str[0]==0){
					int order_number = atoi(str);
					sandwiches.push_back(order_number);
					// cout << "Order: " << order_number << endl;
				}

			}

			/* Add sandwiches to the list */
			totalSandwichList.push_back(sandwiches);

			/* Empty the vector */
			while(sandwiches.empty()){
				sandwiches.pop_back();
			}

			/* create thread for each cashier */
			thread_create((thread_startfunc_t) cashier,(void*)(i));
		}

	}

