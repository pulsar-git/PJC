#ifndef _OPERANDS_H
#define _OPERANDS_H

#pragma once
#include "base_operand.h"
#include "base_parameter.h"

class saved_state;

class operand_two : public base_operand
{
public:
	operand_two(){_param[0]=0;_param[1]=0;};
	operand_two(base_parameter *p1,base_parameter *p2){_param[0]=p1;_param[1]=p2;};
	virtual ~operand_two(){delete _param[0];delete _param[1];};
	virtual char *tostr();
	void setparameter(base_parameter *p,int i){_param[i]=p;};
	base_parameter* getparameter(int i){return _param[i];};
	virtual int parameter_flags(int i);
	virtual void generate(int flags);
	virtual base_operand *reverse();

private:
	base_parameter *_param[2];
	static char _buffer[255];
};

class operand_two_rev : public operand_two
{
public:
	virtual base_operand *reverse();
};

class operand_one : public base_operand
{
public:
	operand_one(){_param[0]=0;};
	operand_one(base_parameter *p1){_param[0]=p1;};
	virtual ~operand_one(){delete _param[0];};
	char *tostr();
	virtual base_operand *reverse();
	void setparameter(base_parameter *p,int i){_param[0]=p;};
	base_parameter* getparameter(int i){return _param[0];};
	virtual void generate(int flags);
	virtual int parameter_flags(int i);

private:
	base_parameter *_param[1];
	static char _buffer[255];
};


class operand_add : public operand_two
{
public:
	virtual void generate(int flags);
	virtual char *name(){return "add %s,%s\n";};
	base_operand *reverseop();
	static char _name[];
};

class operand_sub : public operand_two
{
public:
	virtual void generate(int flags);
	virtual char *name(){return "sub %s,%s\n";};
protected:
	base_operand *reverseop();
	static char _name[];
};

class operand_xor : public operand_two
{
public:
	virtual void generate(int flags);
	virtual char *name(){return "xor %s,%s\n";};
protected:
	base_operand *reverseop();
	static char _name[];
};

class operand_ror : public operand_two
{
public:
	virtual void generate(int flags);
	virtual char *name(){return "ror %s,%s\n";};
	virtual int parameter_flags(int i);
protected:
	base_operand *reverseop();
	static char _name[];
};

class operand_rol : public operand_two
{
public:
	virtual void generate(int flags);
	virtual char *name(){return "rol %s,%s\n";};
	virtual int parameter_flags(int i);
protected:
	base_operand *reverseop();
	static char _name[];
};

class operand_mov : public operand_two_rev
{
	virtual char *name(){return "mov %s,%s\n";};
	virtual bool reversible(){return false;};
	virtual void generate(int flags);
	virtual int parameter_flags(int i);
protected:
	base_operand *reverseop();
	static char _name[];
};

class operand_push : public operand_one
{
public:
	operand_push(){op(op::OP_PUSH);};
	~operand_push();
	virtual char *name(){return "push %s\n";};
	virtual void generate(int flags);
	virtual base_operand *reverse();

protected:
	base_operand *reverseop();
	saved_state *_saved_state;
	static char _name[];
};

class operand_pop : public operand_one
{
public:
	virtual void generate(int flags);
	virtual char *name(){return "pop %s\n";};
protected:
	base_operand *reverseop();
	static char _name[];
};


#endif