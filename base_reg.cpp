#include "base_reg.h"
#include <string.h>
#include "registers.h"

namespace regs
{
	char regs_name [][8]= { "EAX","EBX","ECX","EDX","ESI","EDI","EBP","ESP","AX","BX","CX","DX","SI","DI","BP","SP","AH","BH","CH","DH","AL","BL","CL","DL","MAX_REG"};
}

base_reg::base_reg(void)
{
	parameters(parameters::REGISTER);
}

base_reg::base_reg(int *a)
{
	parameters(parameters::REGISTER);
	this->_value=a;
}

base_reg::~base_reg(void)
{
}

void base_reg::setprotected(bool b)
{
	if(!b)
		registers::instance().rmprotected(_reg);
	else
		registers::instance().setprotected(_reg);
}

bool base_reg::isprotected(void)
{
	return registers::instance().isprotected(_reg);
}

base_reg* base_reg::copy()
{
	base_reg *tmp =new base_reg(this->_value);
	tmp->reg_id(this->reg_id());
	tmp->setvalue(this->_value);
	tmp->_setprotected(this->_protected);
	return tmp;
};

