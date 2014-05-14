#ifndef _NUMBERS_H
#define _NUMBERS_H
#pragma once
#include <boost/pool/detail/singleton.hpp>
#include <boost/random/mersenne_twister.hpp>

class parameter_number;

class numbers
{
public:
	static numbers& instance(void){ return boost::details::pool::singleton_default<numbers>::instance();};
	numbers(void);
	~numbers(void);

	parameter_number *randomnumber(int flags);
private:
	unsigned int random(unsigned int n);
	boost::mt19937 rng;
};
#endif