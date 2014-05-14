#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_
#include "common.h"

class p_exception
{
public:
	p_exception(excp::exception a):_id(a){};
	excp::exception id(){return _id;};
private:
	excp::exception _id;
};

#endif