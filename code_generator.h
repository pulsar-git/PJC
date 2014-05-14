#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H
#pragma once
#include <boost/pool/detail/singleton.hpp>

class code_generator
{
public:
	static code_generator& instance(void){ return boost::details::pool::singleton_default<code_generator>::instance();};
	code_generator(void);
	~code_generator(void);
	void generate(void);
};
#endif