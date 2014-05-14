#ifndef _COMMON_H
#define _COMMON_H

namespace size
{
	enum param_size { BIT_8, BIT_16, BIT_32,MAX_SIZE,ERROR_SIZE};
}
namespace regs
{
	enum regs { EAX,EBX,ECX,EDX,ESI,EDI,EBP,ESP,AX,BX,CX,DX,SI,DI,BP,SP,AH,BH,CH,DH,AL,BL,CL,DL,MAX_REG};
	char regs_name [][8];
}
namespace parameters
{
	enum parameters { REGISTER,NUMBER,MEMORY,MAX_PARAMETERS,ERROR_PARAM};
}
namespace excp
{
	enum exception { NO_REGISTER_FREE,NO_METHOD_AVAILABLE,STACK_OVERFLOW,ILLEGAL_OPERAND};
}
namespace op
{
	enum operands {OP_DEFAULT,OP_PUSH,OP_ERROR};
}

#define PARAMETERS_NOT_PROTECTED	0x01 //in case the operand is not reversible...
#define PARAMETERS_8				0x02
#define PARAMETERS_16				0X04
#define PARAMETERS_32				0X08
#define PARAMETERS_NUMBER			0X20
#define PARAMETERS_REG				0X40
#define PARAMETERS_MEM				0X80

#endif