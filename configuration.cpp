#include ".\configuration.h"
#include "LeoIni.h"
#include <iostream>

namespace config
{
	char min_operand[]="min_operand";
	char max_operand[]="max_operand";
	char protected_register[]="protected_register";
	char templates[]="templates";
	char junk_template[]="junk_template";
	char disabled_template[]="disabled_template";
	char max_stack[]="max_stack";
}

configuration::configuration(void)
{
	ini = 0;
	ini = new Leo::Ini("PJC.ini");
}

configuration::~configuration(void)
{
	delete ini;
}

char *configuration::value(char *name,char *categ)
{
	Leo::Ini::Element el( Leo::Ini::Element::KEY, categ, name );
	if(ini->read(el))
		strcpy(_buffer,el.value.c_str());
	else
	{
		std::cout << "Warning: Can't find ["<<categ<<"]->"<<name<<" in .ini" << std::endl;
		return "0";
	}
	return _buffer;
}
int configuration::intvalue(char *name,char *categ)
{
	return atoi(value(name,categ));
}