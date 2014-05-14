#ifndef _JUNK_LOADER_H
#define _JUNK_LOADER_H

#pragma once
#include "junk_manager.h"
#include "RecurseDir.h"
#include <string>


class junk_template;
class junk_variable;

class junk_loader : public CRecurseDir<WIN32_FIND_DATA>
{
public:
	junk_loader(void);
	~junk_loader(void);
	/**
	* Load all junk_template from a specified directory
	*/
	void load_files(char *directory);
	void HandleFile(WIN32_FIND_DATA *pFile);

private:
	/**
	* Load a junk_template from a file
	*/
	junk_template *load_file(char *file);
	junk_variable *make_var(char *desc);
	std::string directory;
};
#endif
