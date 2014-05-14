#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_
#include <boost/pool/detail/singleton.hpp>

#pragma once
namespace Leo
{
	class Ini;
}

namespace config
{
	char min_operand[];
	char max_operand[];
	char protected_register[];
	char templates[];
	char junk_template[];
	char disabled_template[];
	char max_stack[];
}

class configuration
{
public:
	static configuration& instance(void){ return boost::details::pool::singleton_default<configuration>::instance();};
	configuration(void);
	~configuration(void);
	char *value(char *name,char *categ="random");
	int intvalue(char *name,char *categ="random");
private:
	Leo::Ini *ini;
	char _buffer [255];
};
#endif