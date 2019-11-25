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
	char * LoadShader(const char * filename) const;
	void CompileShader(unsigned const int shader, const char * data) const;
	void CompileProgram(unsigned int & program, unsigned const int & vertexShader, unsigned const int & fragmentShader) const;
};

#endif __ModuleProgram_H__