#include ".\generator_operand.h"
#include "base_operand.h"
#include "operands.h"
#include "regs.h"
#include "registers.h"
#include "parameter_number.h"
#include "numbers.h"
#include "state.h"

#include "windows.h"
#include <iostream>
#include "exceptions.h"

generator_operand::generator_operand(void)
{
}

generator_operand::~generator_operand(void)
{
}

base_operand *generator_operand::random_operand()
{
	switch(rand()%8)
	{
	case 0: return new operand_mov();
	case 1: return new operand_add();
	case 2: return new operand_sub();
	case 3: return new operand_xor();
	case 4: return new operand_ror();
	case 5: return new operand_rol();
	case 6: return new operand_push();
	case 7: return new operand_pop();
	}
	/*switch(rand()%2)
	{
	case 0: return new operand_mov();
	case 1: return new operand_add();
	//case 3: return new operand_xor();
	}*/
	return 0;
}

base_parameter *generator_operand::generate_parameter(int flags)
{
	int methods[2];
	for(int i=0;i<2;i++)
		methods[i]=0;
	
	int j=0;
	if(flags & PARAMETERS_NUMBER)
		methods[j++]=0;
	if(flags & PARAMETERS_REG)
		methods[j++]=1;
    if(!j)
		throw new p_exception(excp::NO_METHOD_AVAILABLE);

	j=rand()%j;
		
	switch(methods[j])
	{
	case 0:return numbers::instance().randomnumber(flags);
	case 1:return registers::instance().randomregister(flags);
	default: return 0;
	}
}

base_operand *generator_operand::generate()
{
	base_operand *tmp=random_operand();
	tmp->generate(0);
	return tmp;
}