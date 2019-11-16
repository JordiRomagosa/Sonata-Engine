#include "ModuleProgram.h"
#include "SDL/SDL.h"
#include "GL/glew.h"


ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init()
{
	char * vertexData = LoadShader("../Shaders/grid.vs");
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	CompileShader(vertexShader, vertexData);

	char * fragmentData = LoadShader("../Shaders/grid.fs");
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	CompileShader(fragmentShader, fragmentData);

	CompileProgram(gridProgram, vertexShader, fragmentShader);

	vertexData = LoadShader("../Shaders/model.vs");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	CompileShader(vertexShader, vertexData);

	fragmentData = LoadShader("../Shaders/model.fs");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	CompileShader(fragmentShader, fragmentData);

	CompileProgram(modelProgram, vertexShader, fragmentShader);

	return true;
}

char * ModuleProgram::LoadShader(char * filename)
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, filename, "rb");

	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		data = (char*)malloc(size + 1);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	else
		LOG("Read shader file failed");

	return data;
}

void ModuleProgram::CompileShader(unsigned int shader, char * data)
{
	glShaderSource(shader, 1, &data, NULL);
	glCompileShader(shader);
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		LOG("Shader compilation failed: %s", infoLog);
		//LOG(data);
	}
	else
		LOG("Shader compilation success");
}

void ModuleProgram::CompileProgram(unsigned int & program, unsigned int & vertexShader, unsigned int & fragmentShader)
{
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		LOG("Program linking failed\n");
	}
	else
		LOG("Program linking success\n");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


