#include ".\junk_manager.h"
#include "junk_template.h"
#include "junk_loader.h"
#include "configuration.h"
#include <iostream>
#include "source_parser.h"

junk_manager::junk_manager(void)
{
	_list.clear();
}

junk_manager::~junk_manager(void)
{
}


void junk_manager::init()
{
	junk_loader loader;
	loader.load_files(configuration::instance().value(config::templates,"path"));
}
void junk_manager::add_junk(junk_template *el)
{
	
	if(!strstr(configuration::instance().value(config::disabled_template,"misc"),el->name.c_str()))
		_list.push_front(el);
	else
		std::cout << "disabled template: "<< el->name << std::endl;
}

char * junk_manager::random_junk()
{
	junk_list::iterator iter;
	int max=0;

	max=_list.size();
	if(max)
	{
		max=rand()%max;
		for(iter=_list.begin();iter!=_list.end();iter++)
		{
			if(!max--)
			{
				*source_parser::instance().output_macro() << ";using template " << (*iter)->name << std::endl;
				return (*iter)->tostr();
			}
		}
	}
	return "";
}