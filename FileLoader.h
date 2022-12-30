#pragma once
#include<fstream>

class FileLoader
{
public:
	static std::string GetFileContents(const char* filename);
};

