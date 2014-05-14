#include ".\base_operand.h"

base_operand::base_operand(void):_op(op::OP_DEFAULT)
{
	_protected=false;
}

base_operand::~base_operand(void)
{
}

base_operand *base_operand::reverse()
{
	return this;
}
