#include ".\source_parser.h"
#include <fstream>
#include <iostream>
#include "string.h"
#include "code_generator.h"
#include "registers.h"

source_parser::source_parser(void)
{
	_macro=0;
}

source_parser::~source_parser(void)
{
}

void source_parser::compute_protected(char *a)
{
	char *tmp=0;
	registers::instance().clear_saved_protection();
	std::cout << "Protection on: ";
	for(int i=0;i<regs::MAX_REG;i++)
	{
		// Ugly code to get protected register...must be preceded by a space!
		if((tmp=strstr(a,regs::regs_name[i])) && (*--tmp==' '))
		{
			registers::instance().set_saved_protection((regs::regs)i,true);
			std::cout  <<regs::regs_name[i] << " ";
		}
	}
	std::cout << "\n";
}

void source_parser::parse(char *filename,char *dest,char *macro)
{
	std::cout << "Parsing " << filename <<"\n";
	char _buffer[500];
	char *buf_ptr=0;
	file_in = new std::fstream(filename,std::ios::in);
	file_out = new std::fstream(dest,std::ios::out);
	file_out_macro = new std::fstream(macro,std::ios::out);

	*file_out_macro << ";#########################################\n";
	*file_out_macro << ";#########         PJC #1        #########\n";
	*file_out_macro << ";#########################################\n";

	*file_out << "include " << macro << "\n";

	bool generate=false;
	while(!file_in->eof())
	{
		file_in->getline(_buffer,500);
		if(buf_ptr=strstr(_buffer,";*#JUNK"))
		{
			strncpy(_buffer,";##",3); //dont do it another time if we reparse the file
			*file_out_macro << ";start of junk\n";
			generate=true;
			*file_out << _buffer << "\n";
			compute_protected(_buffer);
			continue;
		}

		if(generate)
		{
			*file_out_macro << "macro" << _macro << " MACRO\n";
			code_generator::instance().generate();
			//*file_out_macro << "int 3\n";
			*file_out_macro << "ENDM\n\n";

			*file_out << "\t\t\t\t\t\t\t\t\t\tmacro" <<_macro++ <<"\n";
		}

		if(buf_ptr=strstr(_buffer,";*#END JUNK"))
		{
			strncpy(_buffer,";##",3); //dont do it another time if we reparse the file
			generate=false;
			*file_out_macro << ";end of junk\n";
		}

		*file_out << _buffer << "\n";
	}   	

	file_in->close();
	file_out->close();
	file_out_macro->close();
}