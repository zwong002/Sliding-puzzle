#ifndef PROBLEM_H
#define PROBLEM_H

#include <queue>
#include <array>
#include <iostream> 
#include <stdio.h>

using namespace std; 

//struct node{
	//problem data;
	//struct node *next_child;
//};

struct problem{
		static const int PROBLEM_SIZE = 9; //9 values including the 0 as a blank
		bool goal_state;
		int heuristic = 0;
		int cost;
		bool can_right = true;
		bool can_left = true;
		bool can_up = true;
		bool can_down = true; //boolean that state if we should create a node using a slide action
		//OPERATORS
		//struct problem *down, *up, *left, *right; //pointers to the resulting puzzle if a move was made? possibly 
		struct problem* parent;  //keep track of the parent so that if this node is the goal state, you can traverse up the tree to trace it?
		int state[PROBLEM_SIZE];
		
	
	public: 
		problem(int[], int size, int algorithm); //parameters are values to create puzzle, size of puzzle
		problem(int data[], problem parent_node, int action, int algorithm); //parameters: data(parent node), size of puzzle, type of node(slide down, up, left, right)
		problem(const problem &p1); //copy constructor
		problem();
		//HELPERS
		
		bool goal_test(int[]);  //input the state of the node to test if it is equal to solved puzzle
		void print_state();  //print the resulting state after using operator
		void slide_up();
		void slide_down();
		void slide_left();
		void slide_right();
		//void make_node();
		void expand(std::queue<problem>& nodes, problem p, int algorithm);
	
		int find_empty();
		void set_state(int[]);
		void assign_heuristic(int algorithm); //1: Uniform cost, 2: Misplaced tile A*, 3: Manhattan A*
		problem get_parent();
		
		//void add_to_tree( 

};

#endif

