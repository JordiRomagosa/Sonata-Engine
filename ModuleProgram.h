#ifndef __ModuleProgram_H__
#define __ModuleProgram_H__

#include "Module.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();

	unsigned int gridProgram;
	unsigned int modelProgram;

private:
	char * LoadShader(char * filename);
	void CompileShader(unsigned int shader, char * data);
	void CompileProgram(unsigned int & program, unsigned int & vertexShader, unsigned int & fragmentShader);
};

#endif __ModuleProgram_H__