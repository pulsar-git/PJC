#include "parameter_number.h"

parameter_number::parameter_number(void)
{
	parameters(parameters::NUMBER);
}

parameter_number::~parameter_number(void)
{
}

parameter_number::parameter_number(unsigned int a)
{
	parameters(parameters::NUMBER);
	value(a);
}

parameter_number* parameter_number::copy()
{
	parameter_number *tmp=new parameter_number(this->_value);
	tmp->setprotected(this->isprotected());
	return tmp;
}