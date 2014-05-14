#ifndef _BASE_PARAMETER_H
#define _BASE_PARAMETER_H
#pragma once
#include "common.h"

class base_parameter
{
public:
	base_parameter(void);
	~base_parameter(void);
	
	virtual void setprotected(bool b){_protected=b;};
	virtual bool isprotected(void){return _protected;};

	void sethasvalue(bool b){_hasvalue=b;};
	bool hasvalue(void){return _hasvalue;};

	size::param_size size(void){return _size;};
	int size_flags(void);

	virtual unsigned int value(void){return _value;};
	virtual void value(unsigned int a){_value=a;sethasvalue(true);};

	virtual char *tostr();
	virtual base_parameter* copy(){return new base_parameter();};

	void parameters(parameters::parameters i){_param=i;};
	parameters::parameters parameters(){return _param;};

protected:
	char _buffer[20];
	void size(size::param_size s){_size=s;};

	unsigned int _value;
	bool _protected;
	bool _hasvalue;
	size::param_size _size;
	parameters::parameters _param;
};
#endif