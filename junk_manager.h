#ifndef _JUNK_MANAGER_H
#define _JUNK_MANAGER_H

#pragma once
#include <boost/pool/detail/singleton.hpp>
#include <list>
class junk_template;

typedef std::list<junk_template *> junk_list;

class junk_manager
{
public:
	static junk_manager& instance(void){ return boost::details::pool::singleton_default<junk_manager>::instance();};
	junk_manager(void);
	~junk_manager(void);

	void add_junk(junk_template *);
	char * random_junk();
	void init();

private:
	void display_junk(char *);
	//list of templates
	junk_list _list;
};
#endif

