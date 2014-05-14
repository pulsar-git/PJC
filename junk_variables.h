#ifndef _JUNK_VARIABLES_H
#define _JUNK_VARIABLES_H
#pragma once
#include "junk_variable.h"
#include <list>
#include "common.h"

class junk_variable_reg: public junk_variable
{
public:
	junk_variable_reg(){};
	virtual ~junk_variable_reg(){};
	size::param_size size(){return _size;};
protected:
	size::param_size _size;
};

class junk_variable_reg32 :public junk_variable_reg
{
public:
	junk_variable_reg32(void);
	junk_variable_reg32(char *name);
	~junk_variable_reg32(void){};
	virtual char *tostr();
	
};

class junk_variable_reg16:public junk_variable_reg
{
public:
	junk_variable_reg16(void);
	junk_variable_reg16(char *name);
	~junk_variable_reg16(void){};
	virtual char *tostr();
};

class junk_variable_reg8:public junk_variable_reg
{
public:
	junk_variable_reg8(void);
	junk_variable_reg8(char *name);
	~junk_variable_reg8(void){};
	virtual char *tostr();
};

typedef std::list<std::string *> values_list;

class junk_variable_values: public junk_variable
{
public:
	junk_variable_values(void);
	junk_variable_values(char *name,char *values);
	~junk_variable_values(void);
	virtual char *tostr();
private:
	values_list _list;
	
};
#endif