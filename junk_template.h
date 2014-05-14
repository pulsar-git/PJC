#ifndef _JUNK_TEMPLATE_H
#define _JUNK_TEMPLATE_H
#pragma once
#include <string>
#include <list>
#include <map>

class junk_variable;

typedef std::list<junk_variable *> junk_var;
typedef std::map <char *,char *> value_map;

class junk_template
{
public:
	junk_template(void);
	~junk_template(void);

	void template_txt(char *);
	void protect_reg(bool a){_protect_reg=a;};
	bool isreg_protected(){return _protect_reg;};
	char *tostr();
	void add_var(junk_variable *var);
	void init_map();

	std::string name;
	std::string author;
private:
	junk_variable *nextvariable(char *text);
	bool _protect_reg;
	char *_buffer;
	char *_buff_temp;
	junk_var _list;
	value_map _map;
	
};
#endif