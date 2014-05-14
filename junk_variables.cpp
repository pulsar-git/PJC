#include "junk_variables.h"
#include "registers.h"
#include "common.h"
#include <iostream>

junk_variable_reg32::junk_variable_reg32(char *name)
{
	this->_vartype=var::REG;
	this->_size=size::BIT_32;
	this->name(name);
}

char *junk_variable_reg32::tostr()
{
	base_reg *reg1=registers::instance().randomallregister(PARAMETERS_32);
	regs::regs a=reg1->reg_id();
	delete reg1;
	return regs::regs_name[a];
}

junk_variable_reg16::junk_variable_reg16(char *name)
{
	this->_vartype=var::REG;
	this->_size=size::BIT_16;
	this->name(name);
}

char *junk_variable_reg16::tostr()
{
	base_reg *reg1=registers::instance().randomallregister(PARAMETERS_16);
	regs::regs a=reg1->reg_id();
	delete reg1;
	return regs::regs_name[a];
}

junk_variable_reg8::junk_variable_reg8(char *name)
{
	this->_vartype=var::REG;
	this->_size=size::BIT_8;
	this->name(name);
}

char *junk_variable_reg8::tostr()
{
	base_reg *reg1=registers::instance().randomallregister(PARAMETERS_8);
	regs::regs a=reg1->reg_id();
	delete reg1;
	return regs::regs_name[a];
}


junk_variable_values::junk_variable_values(char *name,char *values)
{
	this->_vartype=var::VALUES;
	char _buf[50];
	this->name(name);
	_list.clear();

	int size=strlen(values);
	char *current=values;
	char *current_end=strstr(current," ");
	while(current_end)
	{
		strncpy(_buf,current,current_end-current);
		_buf[current_end-current]='\0';

		_list.push_front(new std::string(_buf));
		current=current_end+1;
		current_end=strstr(current," ");
	}
	strncpy(_buf,current,strlen(current));
	_buf[strlen(current)]='\0';

	_list.push_front(new std::string(_buf));
}
junk_variable_values::~junk_variable_values(void)
{
	values_list::iterator iter;
	for(iter=_list.begin();iter!=_list.end();iter++)
	{
		delete *iter;
	}
}

char *junk_variable_values::tostr()
{
	int i=rand()%_list.size();
	values_list::iterator iter;
	iter=_list.begin();
    while(i--)
		iter++;

	return (char *)(*iter)->c_str();
}