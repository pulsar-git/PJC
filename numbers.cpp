#include "numbers.h"
#include "common.h"
#include "parameter_number.h"
#include <ctime>


numbers::numbers(void)
{
	rng.seed(static_cast<unsigned int>(std::time(0)));
}

numbers::~numbers(void)
{

}

unsigned int numbers::random(unsigned int n)
{
	if(n==0xFFFFFFFF)
		return rng();
	else
		return rng()%(n+1);
}

parameter_number * numbers::randomnumber(int flags)
{
	int tmp_num[3];

	int i=0;
	if(flags & PARAMETERS_8)
		tmp_num[i++]=0xFF;
	if(flags & PARAMETERS_16)
		tmp_num[i++]=0xFFFF;
	if(flags & PARAMETERS_32)
		tmp_num[i++]=0xFFFFFFFF;

	i=rand()%i;

	return new parameter_number(random(tmp_num[i]));
}


