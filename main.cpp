#include "windows.h"
#include <iostream>
#include "registers.h"
#include "base_reg.h"
#include "regs.h"

#include "parameter_number.h"
#include "state.h"
#include <list>
#include "base_operand.h"

#include "operands.h"
#include "generator_operand.h"
#include "time.h"
#include "source_parser.h"

#include "configuration.h"
#include "junk_manager.h"

int main(int argc,char **argv)
{
	std::cout << "#########################################\n";
	std::cout << "#########         PJC #1        #########\n";
	std::cout << "#########################################\n";

	srand(time(0));
	junk_manager::instance().init();

	if(argc==4)
	{
		source_parser::instance().parse(argv[1],argv[2],argv[3]);
	}
	else
		std::cout << "Usage: PJC <source> <dest> <macro file>\n";

}