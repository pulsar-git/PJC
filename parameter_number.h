#ifndef _PARAMETER_NUMBER_H
#define _PARAMETER_NUMBER_H
#pragma once
#include "base_parameter.h"

class parameter_number :
	public base_parameter
{
public:
	parameter_number(void);
	parameter_number(unsigned int a);
	~parameter_number(void);
	bool hasvalue(void){return true;};

	parameter_number* copy();
};

#endif