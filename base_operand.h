#ifndef _BASE_OPERAND_H
#define _BASE_OPERAND_H

#pragma once
#include "common.h"
class base_parameter;

class base_operand
{
public:
	base_operand(void);
	~base_operand(void);
	virtual char *tostr(){return name();};
	virtual char *name(){return "base op\n";};
	virtual base_operand *reverse();
	virtual base_parameter* getparameter(int i){return 0;};
	virtual void setparameter(base_parameter *,int i){};
	virtual bool reversible(){return true;}; //Is the operand reversible? ex: add, sub, xor is , mov is not!
	virtual int parameter_flags(int i){return 0;}
	bool isprotected(){return _protected;}
	void setprotected(){_protected=true;};
	virtual void generate(int flags){};
	op::operands op(){return _op;};
	void op(op::operands a){_op=a;};

protected:
	virtual base_operand *reverseop(){return this;};
	op::operands _op;
private:
	bool _protected;

};
#endif