#ifndef _STATE_H
#define _STATE_H

#pragma once
#include <list>
#include <boost/pool/detail/singleton.hpp>
#include "common.h"

class base_operand;

typedef std::list<base_operand *> op_list;

class saved_state
{
public:
	void setprotected(regs::regs i,bool a){_save_protected[i]=a;};
	bool isprotected(regs::regs i){return _save_protected[i];};
protected:
	bool _save_protected[regs::MAX_REG];
};

class state
{
public:
	static state& instance(void){ return boost::details::pool::singleton_default<state>::instance();};
	state(void);
	~state(void);
	void init(void);
	void add_op(base_operand *);
	void flush_list();
	op_list &get_list();
	saved_state * get_saved_state();
	void restore_saved_state(saved_state *);
	void flush_till_push();

private:
	op_list _list;
};

#endif