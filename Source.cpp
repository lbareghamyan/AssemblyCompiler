#include "AssemblyCompiler.h"
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Not enough arguments are provided" << std::endl;
		return 1;
	}
	AssemblyCompiler comp(argv[1]);
	comp.writeIntoABinFile("exe.bin");
}