#ifndef _NFA_LIB_NFA_H
#define _NFA_LIB_NFA_H

#include <string>
#include <vector>
#include <functional>
#include <mutex>

/*
	We need states and transitions for each state
	States may be an object, and transitions may be a condition, along with a pointer to another state object
*/

class NextState {
public:
	State* state;
	State* previousState;
	size_t input_idx;
};

class State {
private:
	size_t current_pos;

public:
	std::function<std::vector<NextState>(void* input)> transition_func;

	inline State() : current_pos(0) {}
	inline State(std::function <std::vector<NextState>(void*)> transition) :
		transition_func(transition)
	{}
};

class NFA
{
	friend class State;
	std::vector<State> allocator;
public:
	std::mutex eq_mut;
	std::vector<NextState> execution_queue;
	std::vector<void*> data;
	inline void setData(std::vector<void*>& dat) { this->data = dat; }
	void execute();
	void execute_mt(size_t num_threads);
	State* getNewState() { allocator.push_back(State()); return &allocator.back() }

	NFA() { }
	~NFA() { }
};

#endif