#include "operands.h"
#include <iostream>
#include "base_reg.h"
#include "registers.h"
#include "exceptions.h"
#include "state.h"
#include "generator_operand.h"
#include "stack_manager.h"
 
char operand_two::_buffer[255]="";

char * operand_two::tostr()
	{
		sprintf(_buffer,name(),_param[0]->tostr(),_param[1]->tostr());
		return _buffer;
	};

int operand_two::parameter_flags(int i)
{
	switch(i)
	{
	case 0: return PARAMETERS_8 | PARAMETERS_16 | PARAMETERS_32 | PARAMETERS_REG;
	case 1: return ((this->getparameter(0)->isprotected()?PARAMETERS_NOT_PROTECTED:0) | PARAMETERS_NUMBER | PARAMETERS_REG | this->getparameter(0)->size_flags());
	}
	return 0;
};

void operand_two::generate(int flags)
{
	base_parameter *param1=0;
	param1=generator_operand::instance().generate_parameter(this->parameter_flags(0));
	this->setparameter(param1,0);

	base_parameter *param2=0;
	param2 = generator_operand::instance().generate_parameter(this->parameter_flags(1));	
	this->setparameter(param2,1);

	if(param1->isprotected())
		this->setprotected();

	
	//this->init();

}

base_operand *operand_two::reverse()
{
		base_operand *tmp=reverseop();
        tmp->setparameter(this->getparameter(0)->copy(),0);
		tmp->setparameter(this->getparameter(1)->copy(),1);
		return tmp;
};

base_operand *operand_two_rev::reverse()
{
		base_operand *tmp=reverseop();
        tmp->setparameter(this->getparameter(0)->copy(),1);
		tmp->setparameter(this->getparameter(1)->copy(),0);
		return tmp;
};

/**
*
* template one
*
*/

 
char operand_one::_buffer[255]="";

 
char * operand_one::tostr()
	{
		sprintf(_buffer,name(),_param[0]->tostr());
		return _buffer;
	};

 
base_operand *operand_one::reverse()
{
		base_operand *tmp=reverseop();
        tmp->setparameter(this->getparameter(0)->copy(),0);
		return tmp;
};


int operand_one::parameter_flags(int i)
{
	return PARAMETERS_32 | PARAMETERS_REG;
};

void operand_one::generate(int flags)
{
	base_parameter *param1=0;
	param1=generator_operand::instance().generate_parameter(this->parameter_flags(0));
	this->setparameter(param1,0);

	if(param1->isprotected())
		this->setprotected();

}

/**
*
* Operand add
*
*/

base_operand *operand_add::reverseop()
{
	return new operand_sub();
}

void operand_add::generate(int flags)
{
	operand_two::generate(flags);
	base_parameter * param= this->getparameter(0); 
	if(param->isprotected())
		if(this->getparameter(1)->parameters()==parameters::REGISTER)
		{
			this->getparameter(1)->setprotected(true);

//Illegal operand...
			if(param->parameters()==parameters::REGISTER)
			{
				registers::instance().setprotectedandson(((base_reg *)param)->reg_id());
				if( ((base_reg *)this->getparameter(1))->reg_id() == 
					((base_reg *)param)->reg_id() )
					throw new p_exception(excp::ILLEGAL_OPERAND);
			}
		}
	
	if(param->isprotected() && param->parameters()==parameters::REGISTER)
	{
		base_reg * reg1 = (base_reg *)param;
		registers::instance().setprotectedandson(reg1->reg_id());
	}
}

/**
*
* Operand sub
*
*/
base_operand *operand_sub::reverseop()
{
	return new operand_add();
}

void operand_sub::generate(int flags)
{
	operand_two::generate(flags);

	base_parameter * param= this->getparameter(0); 
	if(param->isprotected())
		if(this->getparameter(1)->parameters()==parameters::REGISTER)
		{
			this->getparameter(1)->setprotected(true);

//Illegal operand...
			if(param->parameters()==parameters::REGISTER)
			{
				registers::instance().setprotectedandson(((base_reg *)param)->reg_id());
				if( ((base_reg *)this->getparameter(1))->reg_id() == 
					((base_reg *)param)->reg_id() )
					throw new p_exception(excp::ILLEGAL_OPERAND);
			}
		}
	
	if(param->isprotected() && param->parameters()==parameters::REGISTER)
	{
		base_reg * reg1 = (base_reg *)param;
		registers::instance().setprotectedandson(reg1->reg_id());
	}
}
/**
*
* Operand xor
*
*/
base_operand *operand_xor::reverseop()
{
	return new operand_xor();
}

void operand_xor::generate(int flags)
{
	operand_two::generate(flags);
	if(this->getparameter(0)->isprotected())
		if(this->getparameter(1)->parameters()==parameters::REGISTER)
			this->getparameter(1)->setprotected(true);
}

/**
*
* Operand ror & rol
*
*/

base_operand *operand_ror::reverseop()
{
	return new operand_rol();
}

int operand_ror::parameter_flags(int i)
{
	switch(i)
	{
	case 0: return PARAMETERS_8 | PARAMETERS_16 | PARAMETERS_32 | PARAMETERS_REG;
	case 1: return (PARAMETERS_8 | PARAMETERS_NUMBER ) ;
	}
	return 0;
};


void operand_ror::generate(int flags)
{
	operand_two::generate(flags);
	base_parameter * param= this->getparameter(0); 
	if(param->isprotected() && param->parameters()==parameters::REGISTER)
	{
		base_reg * reg1 = (base_reg *)param;
		registers::instance().setprotectedandson(reg1->reg_id());
	}
}


base_operand *operand_rol::reverseop()
{
	return new operand_ror();
}

int operand_rol::parameter_flags(int i)
{
	switch(i)
	{
	case 0: return PARAMETERS_8 | PARAMETERS_16 | PARAMETERS_32 | PARAMETERS_REG;
	case 1: return (PARAMETERS_8 | PARAMETERS_NUMBER ) ;
	}
	return 0;
};

void operand_rol::generate(int flags)
{
	operand_two::generate(flags);
	base_parameter * param= this->getparameter(0); 
	if(param->isprotected() && param->parameters()==parameters::REGISTER)
	{
		base_reg * reg1 = (base_reg *)param;
		registers::instance().setprotectedandson(reg1->reg_id());
	}

}

/**
*
* Operand mov
*
*/

base_operand *operand_mov::reverseop()
{
	this->getparameter(0)->setprotected(false);
	base_reg *tmp =(base_reg *) this->getparameter(1);
	registers::instance().setprotectedandson(tmp->reg_id());
	//std::cout << "rev->" <<this->getparameter(0)->tostr() << " [ " <<this->getparameter(0)->isprotected() << " ] -"<<this->getparameter(1)->tostr() << " [ " <<this->getparameter(1)->isprotected() << " ] " << std::endl;
	return new operand_mov();
}

void operand_mov::generate(int flags)
{
	operand_two::generate(flags);
	if(this->getparameter(1)->isprotected() && this->getparameter(1)->parameters()==parameters::REGISTER)
	{
		//std::cout << "mov " << this->getparameter(0)->tostr() << " "<<  this->getparameter(1)->tostr() << std::endl;
		/**
		* ATTENTION
		* mov edx,edx -> on supprime la protection d'edx si protege...
		*
		*/
		//this->getparameter(0)->setprotected(false);
		//this->getparameter(0)->setprotected(true);
		this->getparameter(1)->setprotected(false);
		

		base_reg *tmp =(base_reg *) this->getparameter(0);
		registers::instance().setprotectedandson(tmp->reg_id());
		
		//std::cout << this->getparameter(0)->tostr() << " [ " <<this->getparameter(0)->isprotected() << " ] -"<<this->getparameter(1)->tostr() << " [ " <<this->getparameter(1)->isprotected() << " ] " << std::endl;

		setprotected();
	}
}


int operand_mov::parameter_flags(int i)
{
	switch(i)
	{
	case 0: return PARAMETERS_8 | PARAMETERS_16 | PARAMETERS_32 | PARAMETERS_REG |PARAMETERS_NOT_PROTECTED;
	case 1: return (this->getparameter(0)->size_flags() | PARAMETERS_NUMBER | PARAMETERS_REG) ;
	}
	return 0;
};


/**
*
* Operand push
*
*/

base_operand *operand_push::reverseop()
{
	return new operand_pop();
}


operand_push::~operand_push()
{
	delete _saved_state;
};

void operand_push::generate(int flags)
{
	operand_one::generate(flags);
	base_parameter * param= this->getparameter(0); 

	_saved_state = state::instance().get_saved_state();
	registers::instance().rmprotected(((base_reg *)param)->reg_id());
	stack_manager::instance().push_it();
	setprotected();
}


base_operand *operand_push::reverse()
{
		operand_pop *tmp=new operand_pop();
        tmp->setparameter(this->getparameter(0)->copy(),0);
		
		state::instance().restore_saved_state(_saved_state);
		stack_manager::instance().pop_it();
		return tmp;
}
/**
*
* Operand pop
*
*/

base_operand *operand_pop::reverseop()
{
	return new operand_push();
}

void operand_pop::generate(int flags)
{
	//This one ISNT protected! so dont save it!
	stack_manager::instance().till_pop_it();
	throw new p_exception(excp::ILLEGAL_OPERAND);
}


