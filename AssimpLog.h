#ifndef __AssimpLog_H__
#define __AssimpLog_H__

#include "Globals.h"

#include <Assimp/LogStream.hpp>
#include <string>

class AssimpLog : public Assimp::LogStream
{
public:
	void write(const char* message)
	{
		std::string str = message;
		std::size_t characterError = str.find_first_of("%\\");
		while (characterError != std::string::npos)
		{
			str[characterError] = '%\\';
			characterError = str.find_first_of("%\\", characterError + 1);
		}
		LOG(str.c_str());
	}
};

#endif __AssimpLog_H__