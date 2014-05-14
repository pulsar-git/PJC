#include ".\stack_manager.h"
#include "configuration.h"
#include "state.h"
#include "exceptions.h"
#include <iostream>

stack_manager::stack_manager(void):_max_stack(0),_cur_stack(0)
{
	_max_stack=configuration::instance().intvalue(config::max_stack,"misc");
}

stack_manager::~stack_manager(void)
{
}
void stack_manager::init()
{
	_cur_stack=0;
}
void stack_manager::push_it()
{
	if(_cur_stack > _max_stack)
	{
		_cur_stack=0;
		//std::cout << "push_it -> illegal operand\n";
		throw new p_exception(excp::STACK_OVERFLOW);
	}

	_cur_stack++;
	//std::cout << "_cur_stack -> " << _cur_stack << std::endl;
}

void stack_manager::pop_it()
{
	if(_cur_stack>0)
	{
		_cur_stack--;
		//std::cout << "_cur_stack -> " << _cur_stack << std::endl;
	}
}
void stack_manager::till_pop_it()
{
	// A bug that appear when we call pop_it directly...we enter in a endless loop
	if(_cur_stack>0)
	{
		//std::cout << "_cur_stack till_pop_it -> " << _cur_stack << std::endl;
		state::instance().flush_till_push();
		//std::cout << "_cur_stack till_pop_it -> " << _cur_stack << std::endl;
	}
}

