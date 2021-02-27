/*
 * main.cpp
 * 
 * Copyright 2021 zacha <zacha@DESKTOP-DN5CCAS>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <queue> 
#include <cstdbool>
#include <vector> 
#include <array>
#include <iostream> 
#include <stdio.h>
#include <ctime>
#include <algorithm> 
#include <random>
#include <chrono>
using namespace std; 
//#include "search.h" 
#include "problem.h"
#include "time.h"

problem general_search(problem p, int queueing_function);
void sort_queue(std::queue<problem>& nodes, problem p, int queueing_function);

int main(int argc, char **argv)
{
	
	bool testing = false;
	int puzzle_size = 9;
	int puzzle_start[puzzle_size];
	int position = 0;
	int search_algorithm;
	int puzzle_type;
	
	cout << "Type 1 to use a random puzzle, or 2 to enter your own values: " << endl;
	cin >> puzzle_type;
	if(puzzle_type == 1){
		int default_puzzle[puzzle_size]  = {1,2,3,4,5,6,7,8,0};
		//unsigned seed = 0;
		unsigned seed = 0;
		std::srand(seed);
		
		shuffle(&default_puzzle[0], &default_puzzle[puzzle_size], default_random_engine(seed));
		for(int i = 0; i < puzzle_size; i++){
			puzzle_start[i] = default_puzzle[i];
		}
		
		for(int i = 0; i < puzzle_size; i++){
			cout << puzzle_start[i]; 
		}
	}else{
		cout << "Enter your values, with 0 representing an empty space: " << endl;
		while(position < puzzle_size){
			int number;
			cin >> number; 
			puzzle_start[position] = number; 
			position++;
		
		}
	}
	
	cout << "What search algorithm would you like to use? " << endl; 
	cout << "Uniform cost: 1" << endl << "Misplaced tile: 2" << endl << "Manhattan: 3" << endl; 
	cin >> search_algorithm;
	if(search_algorithm <= 0 || search_algorithm >=  4){
		cout << "Incorrect input, exiting: " << endl;
		return 0 ;
	}
	
	problem* prob = new problem(puzzle_start, puzzle_size, search_algorithm);
	prob->print_state();
	problem solution = general_search(*prob, search_algorithm);
	
	
	//TESTING SECTION BELOW
	
	if(testing == true){

		 problem *pointer = &solution;
	 }
		 

		
		
	return 0;
}
problem general_search(problem p, int queueing_function){
	//Timer t = Timer();
	//t.setInterval([&]() 
	
	bool testing = false;
	p.assign_heuristic(queueing_function);
	
		 //TO DO: Find which children have lowest f(n), expand those children, implement queue
		 
		
		std::queue<problem> nodes; //initialize queue
		int max_queue_size = 1;
		int nodes_expanded = 0;
		nodes.push(p);
		clock_t start_time = clock();   //measures when code started running
		while(true){
			if(((double)(clock() - start_time) / CLOCKS_PER_SEC) >= 600){  //ten minute runtime
				cout << "Ran out of time, exiting search. " << endl;
				break;
			}
			cout << "----------------------------------------------------------------" << endl;
			if(nodes.empty()){
				cout << "Queue is empty, no solution!" << endl;
				return p;
			}
			cout << "Queue size: " << nodes.size() << endl;
			sort_queue(nodes, p, queueing_function);
			cout << "Best available node in queue to expand is: " << endl;
			nodes.front().print_state();
			problem curNode = nodes.front(); //keep track of the node we are about to pop off, to examine the state of this node
			nodes.pop(); //pop off head element
			
			//cout << "Current node head: " << endl;
			//curNode.print_state();
			if(curNode.goal_state != true){
				
				cout << "Expanding node and adding children to queue, possible children: " << endl << endl << endl;
				//curNode.print_state();
				
				curNode.expand(nodes, curNode, queueing_function); //NEED TO GIVE EXPAND A QUEUE TO FILL? 
				nodes_expanded++;
				
				
			 }else{
				cout << "Found solution: " << endl;
				curNode.print_state();
				problem pointer = curNode; 
				/*while(pointer.parent != NULL){
					problem *temp = pointer.parent;
					pointer = *temp;
					//delete temp;
					//pointer->parent = pointer->parent->parent;
					cout << "Parent of solution: " << endl;
					pointer.print_state();
					//tracer = *tracer.parent;  //iterate up the tree to find the initial state
					
				} 
				* 
				*/
					cout << "Max queue size: " << max_queue_size << endl;
					cout << "Number of nodes expanded: " << nodes_expanded << endl;
					cout << "Solution is at depth: " << curNode.cost << endl;
					cout << "Runtime was: " << (((double)(clock() - start_time)) / CLOCKS_PER_SEC) << endl;
					return curNode;
			 }
			 
			 
			 if(nodes.size() > max_queue_size){
				max_queue_size = nodes.size();
			}
			 
			
		}
	
	if(testing == true){
		p.assign_heuristic(2);
		problem current = problem(p.state, p, 2,2); //sliding down
		current.assign_heuristic(2);
		cout << "Testing general search: slide up" << endl;
		
		cout << "Parent node state: " << endl;
		current.parent->print_state();
		cout << "Child node state: " << endl;
		current.print_state();
		cout << "End state: ";
		
		if(current.goal_state == true){
			cout << "yes" << endl;
		}else{
			cout << "no" << endl;
		}
		cout << "Cost of parent node state: " << current.parent->cost << endl;
		cout << "Cost of child node state: " << current.cost << endl;
		cout << "Number of misplaced tiles parent: " << current.parent->heuristic << endl;
		cout << "Number of misplaced tiles child: " << current.heuristic << endl;
	}
	 return p;
 }
 
 
 //void make_queue_misplaced(problem parent){
	 
	 
 //}
 
 
void sort_queue(std::queue<problem>& nodes, problem p, int queueing_function){

	int queue_size;

	std::queue<problem> temp;
	for(int i = 0; i < 32; i++){      //31 SEMI ARIBITRARY, FIND BETTER VALUE OR SOLUTION  //sorting?
		//cout << "Testing sort function: " << i << endl;
		//queue_size = 0;
		std::queue<problem> iterate = nodes;
		//iterate.push(p);
		while(!iterate.empty()){  //go through queue to check for f(n) <= i
		//for(int j = 0; j < nodes.size(); j++){
			
			//queue_size++;
			problem check = iterate.front(); 
			iterate.pop();

			if((check.cost + check.heuristic) == i){  //lowest value found
				//check = iterate.front();
				//cout << "Testing sort function found lowest value of: " << i << endl;
				temp.push(check);  					//put lowest value found in front of new queue
				//check next in queue to see if it satisfies value we are searching for
			}
			        			
		}
		
		
		
		//iterate = nodes; //refill iterate with either the same queue, or the queue with the lowest cost 
	}
	nodes = temp;
	while(!temp.empty()){
		temp.pop();
	}


}
