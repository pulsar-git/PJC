#ifndef _REGISTERS_H
#define _REGISTERS_H
#pragma once
#include "common.h"
#include "base_reg.h"
#include <boost/pool/detail/singleton.hpp>

class registers
{
public:
	static registers& instance(void){ return boost::details::pool::singleton_default<registers>::instance();};

	registers(void);
	~registers(void);
	void init(void);

	base_reg *getregister(regs::regs i){return _registers[i];};

	base_reg * randomregister(int flags);
	base_reg * randomallregister(int flags); // Change that later!!
	bool isprotected(regs::regs);
	void rmprotected(regs::regs);
	void rmprotectedandson(regs::regs);
	void setprotected(regs::regs r);
	void setprotectedandson(regs::regs r);

	void clear_saved_protection();
	void set_saved_protection(regs::regs,bool);
    
    
private:
	int eax;
	int ebx;
	int ecx;
	int edx;
	int esi;
	int edi;
	int ebp;
	int esp;

	base_reg *_registers[regs::MAX_REG];
	bool isselectable(regs::regs r,int flags);

	bool _save_protected[regs::MAX_REG];
};

#endif
