#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>

class AssemblyCompiler
{
public:
	static const int REG_NUMBER= 8;

	AssemblyCompiler(std::string);
	std::vector<uint32_t> getCode();
	void writeIntoABinFile(const std::string&);

private:
	std::vector<uint32_t> binCode;

	//label name, the line its on
	std::map<std::string, int> labels;

	//consts names and values
	std::map<std::string, int> consts;

	//8 bit registers
	std::map<std::string, uint8_t> Registers = {
		{ "R0", 0 }, { "R1", 1 },
		{ "R2", 2 }, { "R3", 3 },
		{ "R4", 4 }, { "R5", 5 },
		{ "R6", 6 }, { "R7", 7 },
		{ "IN", 8 }, { "OUT", 8 }
	};

	//Instrutions
	std::map<std::string, uint8_t> Instructions = {
		{ "ADD", 0 },
		{ "SUB", 1 },
		{ "AND", 2 },
		{ "OR", 3 },
		{ "NOT", 4 },
		{ "XOR", 5 },
		{ "JE", 32 }, { "JNE", 33 },
		{ "JL", 34 }, { "JLE", 35 },
		{ "JGR", 36 }, { "JGRE", 37 }
	};


	void readFromFile(std::string);
	void ConstsAndLabels(std::string,int);
	void parseInstruction(std::string&,int);
	uint32_t operands(const std::string&, int);
	uint32_t destination(const std::string&, bool);

};

