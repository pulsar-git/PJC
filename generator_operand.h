#ifndef _GENERATOR_OPERAND
#define _GENERATOR_OPERAND

#pragma once
#include "common.h"
#include <boost/pool/detail/singleton.hpp>
class base_operand;
class base_parameter;

class generator_operand
{
public:
	static generator_operand& instance(void){ return boost::details::pool::singleton_default<generator_operand>::instance();};
	generator_operand(void);
	~generator_operand(void);
	base_operand *generate();
	base_parameter *generate_parameter(int flags);
private:
	base_operand *random_operand();

};

#endif