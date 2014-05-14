#include ".\junk_loader.h"
#include "junk_template.h"
#include <fstream>
#include <iostream>
#include "junk_variables.h"



junk_loader::junk_loader(void)
{
}

junk_loader::~junk_loader(void)
{
}

junk_variable *junk_loader::make_var(char *desc)
{
	char name[50];

	char * end_name=strstr(desc," ");
	strncpy(name,desc,end_name-desc);

	if(strstr(desc,"reg32"))
		return new junk_variable_reg32(name);
	if(strstr(desc,"reg16"))
		return new junk_variable_reg16(name);
	if(strstr(desc,"reg8"))
		return new junk_variable_reg8(name);
	//Add here other if...

	//default case , one of the values after it...
	return new junk_variable_values(name,end_name+1);


	return 0;
}

junk_template *junk_loader::load_file(char *file)
{
	std::fstream file_in(file,std::ios::in);
	char line_buffer[500];
	char whole_buffer[5000];
	junk_template *mTemplate = new junk_template();

	char *last=file;
	char *cur=file;
	while(cur=strstr(last,"\\"))
	{
		cur++;
		last=cur;
	}
	mTemplate->name.assign(last);

	for(int i=0;i<5000;i++)
		whole_buffer[i]='\0';

	while(!file_in.eof())
	{
		file_in.getline(line_buffer,500);
		
		//Are we in commentaries lines?
		if(line_buffer[0]!=';')
		{
			//Kinda ugly :/
			strcpy(whole_buffer + strlen(whole_buffer),line_buffer);
			strcpy(whole_buffer + strlen(whole_buffer),"\n");
		}
		else
		{
			if(strstr(line_buffer,";#*AUTHOR"))
				mTemplate->author.assign(strstr(line_buffer," ")+1);

			if(strstr(line_buffer,";#*REG_PROTECT"))
				mTemplate->protect_reg(true);

			if(strstr(line_buffer,";#*VAR"))
				mTemplate->add_var(make_var(strstr(line_buffer," ")+1));
		}
	}
	mTemplate->template_txt(whole_buffer);
	file_in.close();

	return mTemplate;
}

void junk_loader::HandleFile(WIN32_FIND_DATA *pFile) // T being the typename
{
	std::string temp;
	temp.clear();
	temp+=directory;
	temp+="\\"+std::string(pFile->cFileName);
	std::cout << "loading file: " << temp << std::endl;
	junk_manager::instance().add_junk(load_file((char *)temp.c_str()));
}

void junk_loader::load_files(char *directory)
{
	std::cout << "loading directory " << directory << std::endl; 
	this->directory.assign(directory);
	this->Run(directory); 
}