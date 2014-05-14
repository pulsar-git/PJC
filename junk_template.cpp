#include ".\junk_template.h"
#include <iostream>
#include "junk_variables.h"

junk_template::junk_template(void):_buffer(0),_protect_reg(false),_buff_temp(0)
{
	_list.clear();
	_map.clear();
}

junk_template::~junk_template(void)
{
	delete _buffer;
	_buffer=0;
	delete _buff_temp;
	_buff_temp=0;

	junk_var::iterator iter;
	for(iter=_list.begin();iter!=_list.end();iter++)
	{
		delete *iter;
	}
}

void junk_template::template_txt(char *text)
{
	_buffer = (char *) malloc(strlen(text));
	strcpy(_buffer,text);
}

void junk_template::init_map()
{
	_map.clear();
	junk_var::iterator iter;
	for(iter=_list.begin();iter!=_list.end();iter++)
	{
		_map[(*iter)->name()]=(*iter)->tostr();
	}
}

char *junk_template::tostr()
{

	if(!_buff_temp)
		_buff_temp = (char *) malloc ( strlen(_buffer) );

	
	char *current=_buffer;
	char *current_end=current;
	char *dest=_buff_temp;
	init_map();

	junk_variable *var=0;
	if(!(var=nextvariable(current)))
	{
		strcpy(_buff_temp,_buffer);
	}
	else
	{
		while(var=nextvariable(current))
		{
			char *var_rep=_map[var->name()];

			current_end=strstr(current,var->name());

			strncpy(dest,current,current_end-current);
			dest +=current_end-current;
			

			strncpy(dest,var_rep,strlen(var_rep));
			dest+=strlen(var_rep);

			current = current_end + strlen(var->name());
		}
		strcpy(dest,current);
		dest[strlen(current)]='\0';
	}

	
	
	return _buff_temp;
}

junk_variable *junk_template::nextvariable(char *text)
{
	junk_var::iterator iter;
	junk_variable *tmp=0;
	char *min=0;
	char *cur;
	for(iter=_list.begin();iter!=_list.end();iter++)
	{
		char *test=(*iter)->name();
		if(cur=strstr(text,(*iter)->name()))
		{
			if ( cur < min || !min)
			{
				min=cur;
				tmp=*iter;
			}
		}
	}
	return tmp;
}

void junk_template::add_var(junk_variable *var)
{
	_list.push_front(var);
}