#include ".\registers.h"
#include "regs.h"
#include "windows.h"
#include <iostream>
#include "exceptions.h"
#include "configuration.h"

registers::registers(void)
{
	init();
}

registers::~registers(void)
{
}

void registers::clear_saved_protection()
{
	for(int i=0;i<regs::MAX_REG;i++)
		_save_protected[i]=false;
}

void registers::set_saved_protection(regs::regs i,bool a)
{
	_save_protected[i]=a;
}

void registers::init(void)
{
	eax=0;
	ebx=0;
	ecx=0;
	edx=0;
	edi=0;
	esi=0;
	ebp=0;
	esp=0;

	for(int i=0;i<regs::MAX_REG;i++)
	{
		delete _registers[i];
		_registers[i]=0;
	}


	_registers[regs::EAX]=new reg32(regs::EAX,&eax);
	_registers[regs::EBX]=new reg32(regs::EBX,&ebx);
	_registers[regs::ECX]=new reg32(regs::ECX,&ecx);
	_registers[regs::EDX]=new reg32(regs::EDX,&edx);
	_registers[regs::EDI]=new reg32(regs::EDI,&edi);
	_registers[regs::ESI]=new reg32(regs::ESI,&esi);
	_registers[regs::EBP]=new reg32(regs::EBP,&ebp);
	_registers[regs::ESP]=new reg32(regs::ESP,&esp);

    _registers[regs::AX]=new reg16(regs::AX,&eax);
	_registers[regs::BX]=new reg16(regs::BX,&ebx);
	_registers[regs::CX]=new reg16(regs::CX,&ecx);
	_registers[regs::DX]=new reg16(regs::DX,&edx);
	_registers[regs::DI]=new reg16(regs::DI,&edi);
	_registers[regs::SI]=new reg16(regs::SI,&esi);
	_registers[regs::BP]=new reg16(regs::BP,&ebp);
	_registers[regs::SP]=new reg16(regs::SP,&esp);

	_registers[regs::AH]=new reg8h(regs::AH,&eax);
	_registers[regs::AL]=new reg8l(regs::AL,&eax);
	_registers[regs::BH]=new reg8h(regs::BH,&ebx);
	_registers[regs::BL]=new reg8l(regs::BL,&ebx);
	_registers[regs::CH]=new reg8h(regs::CH,&ecx);
	_registers[regs::CL]=new reg8l(regs::CL,&ecx);
	_registers[regs::DH]=new reg8h(regs::DH,&edx);
	_registers[regs::DL]=new reg8l(regs::DL,&edx);

	for(int i=0;i<regs::MAX_REG;i++)
	{
		_registers[i]->_setprotected(_save_protected[i]);
		_registers[i]->sethasvalue(false);
	}

}

base_reg *registers::randomregister(int flags)
{
	regs::regs random_reg[regs::MAX_REG];
	int j=0;
	bool dont_allow_protect =  rand()%configuration::instance().intvalue(config::protected_register);

	int new_flags = flags;
	if(dont_allow_protect)
		new_flags|= PARAMETERS_NOT_PROTECTED;

	for(int i=0;i<regs::MAX_REG;i++)
	{
			if(isselectable((regs::regs)i,new_flags))
			{
					random_reg[j++]=(regs::regs)i;
			}
	}
	if(!j)
		throw new p_exception(excp::NO_REGISTER_FREE);

	j=rand()%j;


	return _registers[random_reg[j]]->copy();
}

base_reg *registers::randomallregister(int flags)
{
	regs::regs random_reg[regs::MAX_REG];
	int j=0;
	
	for(int i=0;i<regs::MAX_REG;i++)
	{
			if(isselectable((regs::regs)i,flags))
			{
					random_reg[j++]=(regs::regs)i;
			}
	}
	if(!j)
		throw new p_exception(excp::NO_REGISTER_FREE);

	j=rand()%j;


	return _registers[random_reg[j]]->copy();
}



bool registers::isselectable(regs::regs r,int flags)
{
	# pragma warning (disable: 4800)
    bool notprotected=flags & PARAMETERS_NOT_PROTECTED;
	bool reg8 = flags & PARAMETERS_8;
	bool reg16 = flags & PARAMETERS_16;
	bool reg32 = flags & PARAMETERS_32;
	# pragma warning (default: 4800)

	if(r==regs::ESP || r == regs::SP)
		return false;
    
	bool size=false;
	if((_registers[r]->size()==size::BIT_8) && reg8)
		size = true;
	if((_registers[r]->size()==size::BIT_16) && reg16)
		size = true;
	if((_registers[r]->size()==size::BIT_32) && reg32)
		size = true;

	if( size && ((!notprotected) || (notprotected && !_registers[r]->isprotected())))
		return true;
	else
		return false;
	
}

bool registers::isprotected(regs::regs r)
{
	// UG UGLY CHANGE THAT :p
	switch(r)
	{
	case regs::EAX : return (_registers[regs::AH]->_isprotected()||_registers[regs::AL]->_isprotected()||_registers[regs::AX]->_isprotected()||_registers[regs::EAX]->_isprotected());break;
	case regs::EBX : return (_registers[regs::BH]->_isprotected()||_registers[regs::BL]->_isprotected()||_registers[regs::BX]->_isprotected()||_registers[regs::EBX]->_isprotected());break;
	case regs::ECX : return (_registers[regs::CH]->_isprotected()||_registers[regs::CL]->_isprotected()||_registers[regs::CX]->_isprotected()||_registers[regs::ECX]->_isprotected());break;
	case regs::EDX : return (_registers[regs::DH]->_isprotected()||_registers[regs::DL]->_isprotected()||_registers[regs::DX]->_isprotected()||_registers[regs::EDX]->_isprotected());break;
	case regs::EDI : return (_registers[regs::DI]->_isprotected()||_registers[regs::EDI]->_isprotected());break;
	case regs::ESI : return (_registers[regs::SI]->_isprotected()||_registers[regs::ESI]->_isprotected());break;
	case regs::EBP : return (_registers[regs::BP]->_isprotected()||_registers[regs::EBP]->_isprotected());break;
	case regs::ESP : return (_registers[regs::SP]->_isprotected()||_registers[regs::ESP]->_isprotected());break;

	case regs::AX : return (_registers[regs::AH]->_isprotected()||_registers[regs::AL]->_isprotected()||_registers[regs::AX]->_isprotected()||_registers[regs::EAX]->_isprotected());break;
	case regs::BX : return (_registers[regs::BH]->_isprotected()||_registers[regs::BL]->_isprotected()||_registers[regs::BX]->_isprotected()||_registers[regs::EBX]->_isprotected());break;
	case regs::CX : return (_registers[regs::CH]->_isprotected()||_registers[regs::CL]->_isprotected()||_registers[regs::CX]->_isprotected()||_registers[regs::ECX]->_isprotected());break;
	case regs::DX : return (_registers[regs::DH]->_isprotected()||_registers[regs::DL]->_isprotected()||_registers[regs::DX]->_isprotected()||_registers[regs::EDX]->_isprotected());break;
	case regs::DI : return (_registers[regs::DI]->_isprotected()||_registers[regs::EDI]->_isprotected());break;
	case regs::SI : return (_registers[regs::SI]->_isprotected()||_registers[regs::ESI]->_isprotected());break;
	case regs::BP : return (_registers[regs::BP]->_isprotected()||_registers[regs::EBP]->_isprotected());break;
	case regs::SP : return (_registers[regs::SP]->_isprotected()||_registers[regs::ESP]->_isprotected());break;

	case regs::AH : return (_registers[regs::AH]->_isprotected()||_registers[regs::AX]->_isprotected()||_registers[regs::EAX]->_isprotected());break;
	case regs::AL : return (_registers[regs::AL]->_isprotected()||_registers[regs::AX]->_isprotected()||_registers[regs::EAX]->_isprotected());break;
	case regs::BH : return (_registers[regs::BH]->_isprotected()||_registers[regs::BX]->_isprotected()||_registers[regs::EBX]->_isprotected());break;
	case regs::BL : return (_registers[regs::BL]->_isprotected()||_registers[regs::BX]->_isprotected()||_registers[regs::EBX]->_isprotected());break;
	case regs::CH : return (_registers[regs::CH]->_isprotected()||_registers[regs::CX]->_isprotected()||_registers[regs::ECX]->_isprotected());break;
	case regs::CL : return (_registers[regs::CL]->_isprotected()||_registers[regs::CX]->_isprotected()||_registers[regs::ECX]->_isprotected());break;
	case regs::DH : return (_registers[regs::DH]->_isprotected()||_registers[regs::DX]->_isprotected()||_registers[regs::EDX]->_isprotected());break;
	case regs::DL : return (_registers[regs::DL]->_isprotected()||_registers[regs::DX]->_isprotected()||_registers[regs::EDX]->_isprotected());break;
	default: return true;
	}
}

void registers::setprotected(regs::regs r)
{
	_registers[r]->_setprotected(true);
}

void registers::setprotectedandson(regs::regs r)
{
	setprotected(r);
	switch(r)
	{
	case regs::EAX : setprotectedandson(regs::AX);break;
	case regs::EBX : setprotectedandson(regs::BX);break;
	case regs::ECX : setprotectedandson(regs::CX);break;
	case regs::EDX : setprotectedandson(regs::DX);break;
	case regs::EDI : setprotectedandson(regs::DI);break;
	case regs::ESI : setprotectedandson(regs::SI);break;
	case regs::EBP : setprotectedandson(regs::BP);break;
	case regs::ESP : setprotectedandson(regs::SP);break;

	case regs::AX : setprotectedandson(regs::AL);setprotectedandson(regs::AH);break;
	case regs::BX : setprotectedandson(regs::BL);setprotectedandson(regs::AH);break;
	case regs::CX : setprotectedandson(regs::CL);setprotectedandson(regs::AH);break;
	case regs::DX : setprotectedandson(regs::DL);setprotectedandson(regs::AH);break;
	case regs::DI : break;
	case regs::SI : break;
	case regs::BP : break;
	case regs::SP : break;
	}
}

void registers::rmprotected(regs::regs r)
{
	//std::cout << ";rmprotected " << r << "\n";
	// UG UGLY CHANGE THAT :p
	switch(r)
	{
	case regs::EAX : _registers[regs::EAX]->_setprotected(false);rmprotected(regs::AX);break;
	case regs::EBX : _registers[regs::EBX]->_setprotected(false);rmprotected(regs::BX);break;
	case regs::ECX : _registers[regs::ECX]->_setprotected(false);rmprotected(regs::CX);break;
	case regs::EDX : _registers[regs::EDX]->_setprotected(false);rmprotected(regs::DX);break;
	case regs::EDI : _registers[regs::EDI]->_setprotected(false);rmprotected(regs::DI);break;
	case regs::ESI : _registers[regs::ESI]->_setprotected(false);rmprotected(regs::SI);break;
	case regs::EBP : _registers[regs::EBP]->_setprotected(false);rmprotected(regs::BP);break;
	case regs::ESP : _registers[regs::ESP]->_setprotected(false);rmprotected(regs::SP);break;

	case regs::AX : _registers[regs::AX]->_setprotected(false);rmprotected(regs::AH);rmprotected(regs::AL);break;
	case regs::BX : _registers[regs::BX]->_setprotected(false);rmprotected(regs::BH);rmprotected(regs::BL);break;
	case regs::CX : _registers[regs::CX]->_setprotected(false);rmprotected(regs::CH);rmprotected(regs::CL);break;
	case regs::DX : _registers[regs::DX]->_setprotected(false);rmprotected(regs::DH);rmprotected(regs::DL);break;
	case regs::DI : _registers[regs::DI]->_setprotected(false);break;
	case regs::SI : _registers[regs::SI]->_setprotected(false);break;
	case regs::BP : _registers[regs::BP]->_setprotected(false);break;
	case regs::SP : _registers[regs::SP]->_setprotected(false);break;

	case regs::AH : _registers[regs::AH]->_setprotected(false);break;
	case regs::AL : _registers[regs::AL]->_setprotected(false);break;
	case regs::BH : _registers[regs::BH]->_setprotected(false);break;
	case regs::BL : _registers[regs::BL]->_setprotected(false);break;
	case regs::CH : _registers[regs::CH]->_setprotected(false);break;
	case regs::CL : _registers[regs::CL]->_setprotected(false);break;
	case regs::DH : _registers[regs::DH]->_setprotected(false);break;
	case regs::DL : _registers[regs::DL]->_setprotected(false);break;
	default: break;
	}
}