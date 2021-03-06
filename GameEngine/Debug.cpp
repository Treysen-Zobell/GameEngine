
#include "Debug.h"

#include <iostream>

void Debug::fileLoad(const char* filename, const std::uint_fast32_t 
	line,std::string loadedFile, std::string type, bool success, std::string info)
{
	if (Debug::debug)
	{
		if (!success)
			std::cout << "[ERROR] Could not load file {type:" << type << ", filename:" << loadedFile << "}" << std::endl;
		else
			std::cout << "[DEBUG] Loaded file {type:" << type << ", filename:" << loadedFile << "}" << std::endl;

		if (info != "")
			std::cout << "        " << info << std::endl;
		std::cout << "        " << filename << " : " << line << std::endl;
	}
}

void Debug::alcOpenDevice(const char* filename, const std::uint_fast32_t line,
	bool success, std::string info)
{
	if (Debug::debug)
	{
		if (!success)
			std::cout << "[ERROR] Could not open audio device" << std::endl;
		else
			std::cout << "[DEBUG] Opened audio device" << std::endl;

		if (info != "")
			std::cout << "        " << info << std::endl;
		std::cout << "        " << filename << " : " << line << std::endl;
	}
}

void Debug::alcCreateContext(const char* filename, const std::uint_fast32_t line,
	bool success, std::string info)
{
	if (Debug::debug)
	{
		if (!success)
			std::cout << "[ERROR] Could not create alc context" << std::endl;
		else
			std::cout << "[DEBUG] Created alc context" << std::endl;

		if (info != "")
			std::cout << "        " << info << std::endl;
		std::cout << "        " << filename << " : " << line << std::endl;
	}
}

void Debug::alcMakeContextCurrent(const char* filename, const std::uint_fast32_t line,
	bool success, std::string info)
{
	if (Debug::debug)
	{
		if (!success)
			std::cout << "[ERROR] Could not make alc context current" << std::endl;
		else
			std::cout << "[DEBUG] Made alc context current" << std::endl;

		if (info != "")
			std::cout << "        " << info << std::endl;
		std::cout << "        " << filename << " : " << line << std::endl;
	}
}

const bool Debug::debug = true;
