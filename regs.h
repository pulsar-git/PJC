#ifndef _REGS_H
#define _REGS_H
#pragma once
#include "base_reg.h"
#include "common.h"

class reg32 : public base_reg
{
public:
	reg32(void){size(size::BIT_32);};
	reg32(regs::regs a,int *b){reg_id(a);size(size::BIT_32);setvalue(b);}
	~reg32(void){};
	reg32* copy(){reg32 *tmp =new reg32(this->_reg,this->_value);tmp->setvalue(this->_value);tmp->_setprotected(this->_protected);return tmp;};
};

class reg16 : public base_reg
{
public:
	reg16(void){size(size::BIT_16);};
	~reg16(void){};
	reg16(regs::regs a,int *b){reg_id(a);size(size::BIT_16);setvalue(b);}
	unsigned int value(void){return (*_value)&0x0000FFFF;};
	void value(int a){*_value=(*_value&0xFFFF0000)|(a&0x0000FFFF);};
	reg16* copy(){reg16 *tmp =new reg16(this->_reg,this->_value);tmp->setvalue(this->_value);tmp->_setprotected(this->_protected);return tmp;};
};

class reg8h : public base_reg
{
public:
	reg8h(void){size(size::BIT_8);};
	~reg8h(void){};
	reg8h(regs::regs a,int *b){reg_id(a);size(size::BIT_8);setvalue(b);}
	unsigned int value(void){return ((*_value)&0x0000FF00)>>8;};
	void value(int a){*_value=(*_value&0xFFFF00FF)|(a<<8&0x0000FF00);};
	reg8h* copy(){reg8h *tmp =new reg8h(this->_reg,this->_value);tmp->setvalue(this->_value);tmp->_setprotected(this->_protected);return tmp;};
};
class reg8l : public base_reg
{
public:
	reg8l(void){size(size::BIT_8);};
	~reg8l(void){};
	reg8l(regs::regs a,int *b){reg_id(a);size(size::BIT_8);setvalue(b);}
	unsigned int value(void){return (*_value)&0x000000FF;};
	void value(int a){*_value=(*_value&0xFFFFFF00)|(a&0x000000FF);};
	reg8l* copy(){reg8l *tmp =new reg8l(this->_reg,this->_value);tmp->setvalue(this->_value);tmp->_setprotected(this->_protected);return tmp;};
};

#endif
