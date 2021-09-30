
#include "problem.h" 

using namespace std;

problem::problem(){
	parent = NULL;
}

problem::problem(int data[], int size, int algorithm) { //initial puzzle creating
	goal_state = goal_test(data);
	if(size == PROBLEM_SIZE){
		set_state(data);
	}
	assign_heuristic(algorithm);
	parent = NULL;
	
	cost = 0;
}

problem::problem(int data[], problem parent_node, int action, int algorithm ){ 
	//1:  DOWN 
	//2:  UP
	//3: LEFT
	//4: RIGHT
	this->parent = &parent_node;   //set pointer to point to address of inputted parent node
	this->cost = parent_node.cost + 1;
	
	set_state(data); 
	
	if(action == 1){ //slide down(meaning a tile moves up 3 units which will make it go down in position in 3x3)
		slide_down();
	}else if(action == 2){
		slide_up();
	}else if(action == 3){
		slide_left();
	}else if(action == 4){
		slide_right();
	}
	
	
	assign_heuristic(algorithm);
	goal_state = goal_test(state);
	
	
}

	problem::problem(const problem& p1) {
		problem* n = new problem;
		n->set_state(const_cast<int*>(p1.state));
		set_state(const_cast<int*>(n->state));
		heuristic = p1.heuristic;
		cost = p1.cost;
		parent = p1.parent;
		can_right = p1.can_right;
		can_left = p1.can_left;
		can_up = p1.can_up;
		can_down = p1.can_down;
		
		goal_state = goal_test(state);
}


// --------------------HELPERS------------------
problem problem::get_parent(){
	return *parent;
}
	


bool problem::goal_test(int data[]){
	goal_state = true;
	
	for(int i = 0; i < PROBLEM_SIZE - 1; i++){  //go through the puzzle of whatever size to check if it is solved
		if(data[i] != (i + 1)){
			goal_state = false;
			return goal_state;
		}
	}
	return goal_state;
}

void problem::print_state(){  //input what operator you are using--no input rn
	
	cout << "Node: " << endl;
	for(int i = 0; i < PROBLEM_SIZE; i++){
		if(state[i] <= 0 || state[i] > 8){
			state[i] = 0;
			cout << " * ";
		}else{
			cout << " " << state[i] << " ";
		}
		if( (i + 1) % 3 == 0){
			cout << endl;
		}
	}
	cout << "Depth is: " << cost << endl;
	cout << "Cost of node: " << cost << endl;
	cout << "Heuristic of node is: " << heuristic << endl << endl << endl;
}

void problem::set_state(int data[]){
	//state = new int[PROBLEM_SIZE];
	for(int i = 0; i < PROBLEM_SIZE; i++){
		state[i] = data[i];
	}
}

int problem::find_empty(){
	int empty_space;
	for(int i = 0; i < PROBLEM_SIZE; i++){ //find the location of the empty slot in the array to see how and what available moves there are
		if(state[i] == 0){
			empty_space = i;
		}
	}
	return empty_space;
}
	
void problem::slide_up(){  //switch elements in array to represent a slide up
	int empty_space = find_empty();
	
	int temp = state[empty_space];
	state[empty_space] = state[empty_space + 3];
	state[empty_space + 3] = temp; 
	can_down = false;
	
}

void problem::slide_down(){
	int empty_space = find_empty();
	
	int temp = state[empty_space];
	state[empty_space] = state[empty_space - 3];
	state[empty_space - 3] = temp; 
	can_up = false;
}
void problem::slide_right(){
	int empty_space = find_empty();
	
	int temp = state[empty_space];
	state[empty_space] = state[empty_space - 1];
	state[empty_space - 1] = temp; 
	can_left = false;
}
void problem::slide_left(){
	int empty_space = find_empty();
	
	int temp = state[empty_space];
	state[empty_space] = state[empty_space + 1];
	state[empty_space + 1] = temp; 
	can_right = false;
}

void problem::assign_heuristic(int algorithm){
	//uniform cost
	if(algorithm == 1){
		heuristic = 0;
		
	}else if(algorithm == 2){ //MISPLACED TILE
		heuristic = 0;
		for(int i = 0; i < PROBLEM_SIZE - 1; i++){
			if(state[i] != (i + 1)){
				if(state[i] != 0){
					heuristic += 1;
				}
			}
				
		}
		if(state[PROBLEM_SIZE - 1] != 0){
			heuristic += 1;
		}
	}else if(algorithm == 3){ //MANHATTAN
		heuristic = 0;
		for(int i = 0; i < PROBLEM_SIZE; i++){
			
			if(state[i] != (i + 1)){ //current tile is misplaced, now need to calculate how many moves to correct position
				
				if(state[i] != 0){
					//int value = state[i];
					int goal_column = state[i] % 3;
					if(goal_column == 0){
						goal_column = 3;
					}
					int goal_row = ((state[i] - 1) / 3) + 1;
					int this_row = (i / 3) + 1; 
					int this_column = (i % 3) + 1;
					int horizontal_difference = std::abs(goal_column - this_column);
					int vertical_difference = std::abs(goal_row - this_row);
					heuristic += horizontal_difference + vertical_difference;
				
				}
			}
		}
	}
	
}

void problem::expand(std::queue<problem>& nodes, problem p, int algorithm){
	int children = 0;	
	//CHECKING POSITION TO DETERMINE WHAT CHILDREN NODES CAN BE MADE
	//TRYING TO QUEUE CHILDREN IN ORDER OF F(N) 
	
	if(find_empty() % 3 == 0){ //array positions 0, 3, 6, which are on the left side, so cannot use slide right because nothing is on the left of it to slide right into open space
		can_right = false;
	}
	if((find_empty() + 1) % 3 == 0){ //empty slot in array positions 2, 5, 8, on right side, cannot use slide left
		 can_left = false;
	}
	if(find_empty() <= 2){  //empty slot in top row, can't slide down
		can_down = false;
	}
	if(find_empty() >= 6){  //empty slot in bottom row, can't slide up
		can_up = false;
	}
	
	//CREATING CHILDREN NODES
	if(can_down == true){
		children++;
		problem down_child = problem(p.state, p , 1, algorithm); //create a problem child(no pun intended lol) that results from sliding down
		down_child.assign_heuristic(algorithm);
		nodes.push(down_child);
		down_child.print_state();
	}
	if(can_up == true){
		children++;
		problem up_child = problem(p.state, p , 2, algorithm); //same for up
		up_child.assign_heuristic(algorithm);
		nodes.push(up_child);
		up_child.print_state();
	}
	if(can_left == true){
		children++;
		problem left_child = problem(p.state, p , 3, algorithm); //left
		left_child.assign_heuristic(algorithm);
		left_child.print_state();
	}
	if(can_right == true){
		children++;
		problem right_child = problem(p.state, p , 4, algorithm);//right
		right_child.assign_heuristic(algorithm);
		nodes.push(right_child);
		right_child.print_state();
	}
	
}
