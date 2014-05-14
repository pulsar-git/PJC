#ifndef _SOURCE_PARSER_H
#define _SOURCE_PARSER_H
#pragma once
#include <boost/pool/detail/singleton.hpp>
#include <fstream>
#include "common.h"


class source_parser
{
public:
	static source_parser& instance(void){ return boost::details::pool::singleton_default<source_parser>::instance();};
	source_parser(void);
	~source_parser(void);
	void parse(char *filename,char *dest,char *macro);
	std::fstream *input(){return file_in;};
	std::fstream *output(){return file_out;};
	std::fstream *output_macro(){return file_out_macro;};
	void compute_protected(char *a);
private:
	int _macro;
	std::fstream *file_in;
	std::fstream *file_out;
	std::fstream *file_out_macro;

};
#endif