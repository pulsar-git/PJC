#ifndef _JUNK_VARIABLE_
#define _JUNK_VARIABLE_
#pragma once
#include <string>
#include "common.h"

namespace var
{
	enum vartype { REG,VALUES,UNKNOWN};
}
class junk_variable
{
public:
	junk_variable(void);
	virtual ~junk_variable(void);
	virtual char *tostr()=0;
	char *name(){return (char *)_name.c_str();};
	void name(char *a){_name.assign(a);};
	var::vartype type(){return _vartype;};
protected:
	std::string _name;
	var::vartype _vartype;
};
#endif
