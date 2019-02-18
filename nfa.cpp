#include "nfa.h"
#include <algorithm>
#include <thread>

void NFA::execute()
{
	NextState n;
 	while (true) {
		// BEGIN CRITICAL SECTION
		{
			std::lock_guard<std::mutex> lock(eq_mut);
			if (execution_queue.empty()) {
				// FAIL CRITICAL SECTION
				return;
			}
			n = execution_queue.back();
			execution_queue.pop_back();
		}
		// END CRITICAL SECTION

		std::vector<NextState> next = n.state->transition_func(data[n.input_idx]);

		// BEGIN CRITICAL SECTION
		{
			std::lock_guard<std::mutex> lock(eq_mut);
			execution_queue.reserve(execution_queue.size() + next.size());
			execution_queue.insert(std::end(execution_queue), std::begin(next), std::end(next));
		}
		//END CRITICAL SECTION
	}
}

template<typename Class, typename Func>
static void runner(void* data) {
}

void NFA::execute_mt(size_t n_threads) {
	for(size_t i = 0; i < n_threads; i++)
		std::thread t(&NFA::execute, this);
}