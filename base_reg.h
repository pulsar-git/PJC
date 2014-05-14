#ifndef _BASE_REG_H
#define _BASE_REG_H

#include "common.h"
#include "base_parameter.h"

#pragma once

class base_reg : public base_parameter
{
public:
	base_reg(void);
	~base_reg(void);
	base_reg(int *);

	virtual void setprotected(bool b);
	virtual bool isprotected(void);

	void _setprotected(bool b){_protected=b;};
	bool _isprotected(void){return _protected;};
	
	void setvalue(int *a){_value=a;};
	virtual unsigned int value(void){return *_value;};
	virtual void value(unsigned int a){*_value=a;sethasvalue(true);};

	virtual char *tostr(){return regs::regs_name[_reg];};
	
	void reg_id(regs::regs a){_reg=a;};
	regs::regs reg_id(){return _reg;};

	virtual base_reg* copy();
	

protected:
	//Set where is the value!
	int *_value;
	regs::regs _reg;
};

#endif