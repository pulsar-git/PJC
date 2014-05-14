#include ".\state.h"
#include "base_operand.h"
#include <iostream>
#include "source_parser.h"
#include "junk_manager.h"
#include "configuration.h"
#include "registers.h"
#include "stack_manager.h"

state::state(void)
{
}

state::~state(void)
{
}

void state::init(void)
{
	for(std::list<base_operand*>::iterator iter=_list.begin();iter!=_list.end();iter++)
	{
		delete *iter;
	}
	_list.clear();
}

void state::add_op(base_operand *op)
{
	*(source_parser::instance().output_macro()) << ";is protected\n";
	_list.push_back(op);
}

op_list &state::get_list()
{
	return _list;
}
void state::flush_till_push()
{
	std::fstream *output = source_parser::instance().output_macro();
	op_list &lst = get_list();

	//*output << ";flush till push (pop)\n";
	base_operand *baseop=0;

	while(1)
	{
		std::list<base_operand*>::iterator iter = --lst.end();
		base_operand * curop=*iter;
		baseop = curop->reverse();
		*output << baseop->tostr();

		if(curop->op()==op::OP_PUSH)
		{
			lst.remove(curop);
			delete curop;
			delete baseop;
			break;
		}
		lst.remove(curop);
		delete curop;
		delete baseop;
	}
	//*output << ";END flush till push (pop)\n";
}

void state::flush_list()
{
	stack_manager::instance().init();
	std::fstream *output = source_parser::instance().output_macro();
	op_list &lst = get_list();
	lst.reverse();
	*output << "\n";
	base_operand *baseop=0;
	for(std::list<base_operand*>::iterator iter=lst.begin();iter!=lst.end();iter++)
	{
		if(!(rand()%configuration::instance().intvalue(config::junk_template)))
			*output << junk_manager::instance().random_junk() << std::endl;

		baseop = (*iter)->reverse();
		*output << baseop->tostr();
		delete *iter;
		delete baseop;
	}
	lst.clear();
	//std::cout << ";Reversed\n";
}

saved_state * state::get_saved_state()
{
	saved_state *tmp=new saved_state();
	for(int i=0;i<regs::MAX_REG;i++)
	{
		tmp->setprotected((regs::regs)i,registers::instance().isprotected((regs::regs)i));
	}
	return tmp;
}

void state::restore_saved_state(saved_state *mState)
{
	for(int i=0;i<regs::MAX_REG;i++)
	{
		if(mState->isprotected((regs::regs)i))
			registers::instance().setprotected((regs::regs)i);
	}
}
