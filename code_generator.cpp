#include ".\code_generator.h"
#include "generator_operand.h"
#include "state.h"
#include <iostream>
#include "base_operand.h"
#include "base_parameter.h"
#include "common.h"
#include "registers.h"

#include "source_parser.h"
#include "exceptions.h"
#include "configuration.h"
#include "junk_manager.h"

code_generator::code_generator(void)
{
}

code_generator::~code_generator(void)
{
}


void code_generator::generate()
{
	int min = configuration::instance().intvalue(config::min_operand);
	int max = configuration::instance().intvalue(config::max_operand);

	int i=0;
	int number=0;
	number = min + rand() % (max-min);
	base_operand * baseop=0;

	registers::instance().init();

	state& mState=state::instance();
	
	std::fstream *output = source_parser::instance().output_macro();
	while(i<number)
	{
		try
		{
			if(!(rand()%configuration::instance().intvalue(config::junk_template)))
				*output << junk_manager::instance().random_junk() << std::endl;

			baseop=generator_operand::instance().generate();
			*output << baseop->tostr();
			//std::cout << "G " <<baseop->tostr() << std::endl;

			if(baseop->isprotected())
				mState.add_op(baseop);
			else
				delete this;

		}
		catch(p_exception *exp)
		{
			switch(exp->id())
			{
			case excp::NO_REGISTER_FREE :
				mState.flush_list();
				registers::instance().init();
				break;
			case excp::STACK_OVERFLOW:
				mState.flush_list();
				registers::instance().init();
				break;
			case excp::ILLEGAL_OPERAND :
				//std::cout << ";Illegal Operand!\n";
				*output << ";ILLEGAL_OPERAND!"<<std::endl;
				break;
			default :std::cout << "Exception caught" << exp->id() << "\n";
			}
		}

		i++;
	}
	mState.flush_list();
}