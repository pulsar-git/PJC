#include ".\base_parameter.h"
#include <stdio.h>

base_parameter::base_parameter(void):_value(0),_size(size::ERROR_SIZE),_protected(false),_hasvalue(false),_param(parameters::ERROR_PARAM)
{
}

base_parameter::~base_parameter(void)
{
}

char *base_parameter::tostr(void)
{
	sprintf(_buffer,"0%Xh",_value);
	return _buffer;
}

int base_parameter::size_flags(void)
{
	switch(_size)
	{
	case size::BIT_8 : return PARAMETERS_8;
	case size::BIT_16 : return PARAMETERS_16;
	case size::BIT_32 : return PARAMETERS_32;
	}
	return 0;
}