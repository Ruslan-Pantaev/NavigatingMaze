/*  Author: Ruslan Pantaev
*   Date: 3/24/17
*   Assignment: HW_2 -CISC_3130
*/


#include <iostream>
#include <fstream>
#include <list>
#include <stack>
#include <queue>

using namespace std;

void print_maze(int *cells, int maze_size);


int main (int argc, char *argv[])
{
	if (argc != 2) {
		cout << "USAGE: " << argv[0] << " <filename>\n" << endl;
	}

	ifstream inFile(argv[1]);
	if (!inFile.is_open()) {
		cout << "File could not be opened, try again\n";
		return 1;
	}
	
	cout << "\n";

	int maze_size;
	try {		
		inFile >> maze_size;
		if (!inFile) throw string("file is empty!\n");
	} catch (string message) {
		cout << "*** Exception thrown: " << message;
	}

	int *cells = new int[maze_size * maze_size];
	for (int i = 0; i < maze_size * maze_size; i++) {
		try {
			inFile >> cells[i];
		} catch (string message) {
		cout << "*** could not populate array" << message;
		}
	}

	inFile.close();
	print_maze(cells, maze_size);

	// -------------------------STACK-------------------------

	stack<int> stk_path;
	// pushing on entrance coordinates
	stk_path.push(0);

	#define VISITED 2
	int current;
	int goal = maze_size * maze_size - 1;
	bool solved = false;
	int *parent = new int[maze_size * maze_size];
	for (int i = 0; i < maze_size * maze_size; i++) {
		parent[i] = 0;
	}
		
	while (!stk_path.empty()) {
		current = stk_path.top();
		stk_path.pop();
		if (current == goal) {
			solved = true;
			// parent[current] = current; // BUG
		}
		
		cells[current] |= VISITED;
		
		// DOWN
		if (current <= (maze_size-1) * maze_size && cells[current+maze_size] == 0) {
			parent[current+maze_size] = current;
			stk_path.push(current+maze_size);
		}
		// RIGHT
		if (current % maze_size != maze_size-1 && cells[current+1] == 0) {
			parent[current+1] = current;
			stk_path.push(current+1);
		}
		// UP
		if (current >= maze_size && cells[current-maze_size] == 0) {
			parent[current-maze_size] = current;
			stk_path.push(current-maze_size);
		}
		// LEFT
		if (current % maze_size != 0 && cells[current-1] == 0) {
			parent[current-1] = current;
			stk_path.push(current-1);
		}
	}

	if (stk_path.empty() && solved == false) {
		cout << "No Solution for Stacks..." << endl;
	} else cout << "Reached Goal with Stacks!" << endl;

	// PRINTING
	// ***Many special thanks to peer Robert for suggesting, explaining and helping with this method***
	// "x and y are "encoded" into the single position integer, and can be decoded thus:" - Robert
	// x = stack.top() % maze_size;
	// y = stack.top() / maze_size;
		
	// Prints reverse w/out stack
	current = goal;
	cout << "(" << goal % maze_size << ", " << goal / maze_size << ") "; // BUG FIX (won't print goal coordinates otherwise)
	while (current != 0) {
		current = parent[current];
		cout << "(" << current % maze_size << ", " << current / maze_size << ") ";
	}
	cout << endl;

	// TO UNREVERSE PRINT
	// stack<int> stk_print;
	// // stk_path = new stack();
	// current = goal;
	// while (current != 0) {
	// 	stk_print.push(current);
	// 	current = parent[current];
	// }

	// cout << "(0,0) ";
	// while (!stk_print.empty()) {
	// 	cout << "(" << stk_print.top() % maze_size << "," << stk_print.top() / maze_size << ") ";
	// 	stk_print.pop();
	// }
	// cout << endl << endl;

	// -------------------------END STACK-------------------------

	// ---------------------------QUEUE---------------------------

	queue<int> que_path;
	// pushing on entrance coordinates
	que_path.push(0);

	// ***allocating unnecessary mem... I know :/
	int current_que;
	solved = false;
	int *parent_que = new int[maze_size * maze_size];
	for (int i = 0; i < maze_size * maze_size; i++) {
		parent_que[i] = 0;
	}
	// RESETTING bit to NOT VISITED WOOOOO
	for (int i = 0; i < maze_size * maze_size; i++) {
		cells[i] &= ~(VISITED);
	}
		
	while (!que_path.empty()) {
		current_que = que_path.front();
		que_path.pop();
		if (current_que == goal) {
			solved = true;
		}
		
		cells[current_que] |= VISITED;
		
		// DOWN
		if (current_que <= (maze_size-1) * maze_size && cells[current_que+maze_size] == 0) {
			parent_que[current_que+maze_size] = current_que;
			que_path.push(current_que+maze_size);
		}
		// RIGHT
		if (current_que % maze_size != maze_size-1 && cells[current_que+1] == 0) {
			parent_que[current_que+1] = current_que;
			que_path.push(current_que+1);
		}
		// UP
		if (current_que >= maze_size && cells[current_que-maze_size] == 0) {
			parent_que[current_que-maze_size] = current_que;
			que_path.push(current_que-maze_size);
		}
		// LEFT
		if (current_que % maze_size != 0 && cells[current_que-1] == 0) {
			parent_que[current_que-1] = current_que;
			que_path.push(current_que-1);
		}
	}

	if (que_path.empty() && solved == false) {
		cout << "No Solution for Queues..." << endl;
	} else cout << "Reached Goal with Queues!" << endl;

	// PRINTING IN REVERSE....
	queue<int> que_print;
	// que_path = new queue();
	current_que = goal;
	while (current_que != 0) {
		que_print.push(current_que);
		current_que = parent_que[current_que];
	}
	
	while (!que_print.empty()) {
		cout << "(" << que_print.front() % maze_size << "," << que_print.front() / maze_size << ") ";
		que_print.pop();
	}
	cout << "(0,0) ";
	cout << endl << endl;

	// -------------------------END QUEUE-------------------------

	
	return 0;
}

void print_maze(int *cells, int maze_size)
{
	for (int i = 0; i < maze_size * maze_size; i++) {		
		if (i % maze_size == 0) cout << endl;
		cout << cells[i] << " ";		
	}
	cout << endl << endl;
}