#ifndef _STACK_MANAGER_H
#define _STACK_MANAGER_H

#pragma once
#include <boost/pool/detail/singleton.hpp>


class stack_manager
{
public:
	static stack_manager& instance(void){ return boost::details::pool::singleton_default<stack_manager>::instance();};
	stack_manager(void);
	~stack_manager(void);
	void push_it();
	void pop_it();
	void till_pop_it();
	int cur_stack(){return _cur_stack;};
	void init();
private:
	int _max_stack;
	int _cur_stack;
};
#endif