#include "AssemblyCompiler.h"
AssemblyCompiler::AssemblyCompiler(std::string filePath)
{
	readFromFile(filePath);
}
std::vector<uint32_t> AssemblyCompiler::getCode()
{
	return binCode;
}
void AssemblyCompiler::writeIntoABinFile(const std::string& filePath)
{
	std::ofstream fs(filePath, std::ios::out | std::ios::binary);
	for (int i = 0; i < binCode.size(); i++)
	{
		fs.write((char*)&binCode[i], sizeof(uint32_t));
	}
	fs.close();
}
std::string toUpper(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		str[i] = std::toupper(str[i]);
	}
	return str;
}
bool isNumber(std::string str)
{
	return !str.empty() && str.find_first_not_of("0123456789") == std::string::npos;
}
void AssemblyCompiler::readFromFile(std::string filePath)
{
	std::ifstream file;
	file.open(filePath);
	if (!file.is_open())
	{
		throw std::runtime_error( "Failed to open the file!");
		exit(1);
	}
	std::string read;
	int line = 0;
	while (std::getline(file,read))//first find consts and labels
	{
		line++;
		//find all labels and consts in the code
		ConstsAndLabels(read, line);
	}
	//get into beginnig of the file
	file.clear();
	file.seekg(0);
	line = 0;
	while (std::getline(file, read))
	{
		line++;
		if (read.find("const") == std::string::npos &&
			read.find("label") == std::string::npos)
		{
			parseInstruction(read, line);
		}
	}
	file.close();
}
void AssemblyCompiler::ConstsAndLabels(std::string str, int line)
{
	std::size_t found = str.find("const");
	if (found != std::string::npos)
	{
		// name of the consr
		size_t k = str.find_first_not_of(" ", found + strlen("const"));
		size_t k2 = str.find_first_of(" ", k);
		if (k == std::string::npos)
		{
			throw std::invalid_argument("No const name specified in line " + std::to_string(line));
		}
		std::string name = toUpper(str.substr(k, k2 - k));
		if (isNumber(name) || name[0] != '%')
		{
			throw std::invalid_argument("Line " + std::to_string(line) + "- const names should start with %");
		}
		//const value
		k = str.find_first_not_of(" ", k2);
		k2 = str.find_first_of(" ", k);
		if (k == std::string::npos)
		{
			throw std::invalid_argument("Line " + std::to_string(line) + ": Consts should have a value");
		}
		std::string number = str.substr(k, k2 - k);
		if (!isNumber(number) || consts.find(name) != consts.end())//check if const val is a number and is not being redifined
		{
			throw std::invalid_argument("Invalid code in line " + std::to_string(line));
		}
		consts.insert({ name, std::stoi(number) });
	}
	else if (str.find("label") != std::string::npos)
	{
		//label name
		found = str.find("label");
		size_t k = str.find_first_not_of(" ", found + strlen("label"));
		size_t k2 = str.find_first_of(" ", k);
		if (k == std::string::npos)
		{
			throw std::invalid_argument("No label name specified in line" + std::to_string(line));
		}
		std::string name = toUpper(str.substr(k, k2 - k));
		if (labels.find(name) != labels.end() || name[0] != '_')
		{
			throw std::invalid_argument("Invalid code in line " + std::to_string(line));
		}
		labels.insert({ name, line });
	}
}
/* gets a line of instruction */
void AssemblyCompiler::parseInstruction(std::string& instruction, int line)
{
	instruction = toUpper(instruction);
	std::string instr;
	uint32_t code = 0;
	size_t k1 = 0, k2 = 0;
	k1 = instruction.find_first_not_of(" ", k1);
	k2 = instruction.find_first_of(" ", k1 + 1);
	instr = instruction.substr(k1, k2 - k1);
	if (Instructions.find(instr) != Instructions.end())//check if its an instruction
	{
		code |= (Instructions[instr] << 24);
		bool isCond = (Instructions[instr] >= 32 && Instructions[instr] <= 37);
		uint32_t operand = 0;
		for (int i = 0; i < 3; i++)
		{
			k1 = instruction.find_first_not_of(" ", k2 + 1);
			k2 = instruction.find_first_of(" ", k1);
			if (k1 == std::string::npos)
			{
				throw std::invalid_argument("Invalid instruction in line" + std::to_string(line));
			}
			instr = instruction.substr(k1, k2 - k1);
			if (i == 2)
			{
				operand = destination(instr, isCond);
			}
			else
			{
				operand = operands(instr, i + 1);
			}
			if (operand == 0xFFFF)
			{
				throw std::invalid_argument("Unkown operand in line " + std::to_string(line));
			}
			code |= operand;
		}
		binCode.push_back(code);
	}
	else 
	{
		throw std::invalid_argument("Unkown argument in line " + std::to_string(line));
	}

}
uint32_t AssemblyCompiler::operands(const std::string& operand, int opNum)
{
	uint32_t code = 0;
	if (Registers.find(operand) != Registers.end())
	{	
		if (operand == "OUT")//out cannot be an operand
		{
			return 0xFFFF;
		}
		code |= (Registers[operand] << ((3 - opNum) * 8));
	}
	else if (consts.find(operand) != consts.end())
	{
		code |= (consts[operand] << ((3 - opNum) * 8));
		code |= code | (1 << (8 - opNum)); // set imm bit  
	}
	else if (isNumber(operand))
	{
		code |= (stoi(operand) << ((3 - opNum) * 8));
		code |= code | (1 << (32 - opNum));//set imm bit
	}
	else 
	{
		return 0xFFFF;//set all bits for unkown operands
	}
	return code;
}

uint32_t AssemblyCompiler::destination(const std::string& dest, bool isCond)
{
	uint32_t code = 0;
	if (!isCond && Registers.find(dest) != Registers.end())
	{

		if (dest == "IN")//IN cannot be a destination
		{
			return 0xFFFF;
		}
		code |= Registers[dest];
	}
	else if (isCond && labels.find(dest) != labels.end())
	{
		code |= labels[dest];
	}
	else 
	{
		return 0xFFFF;//set all bits for unkown destionation
	}
	return code;
}
