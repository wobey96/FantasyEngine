#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>

std::string GetProjectDirectory()
{
	std::string fullPath(__FILE__);
	size_t found = fullPath.find_last_of("/\\");
	if (found != std::string::npos)
	{
		std::string projectDir = fullPath.substr(0, found);

		found = projectDir.find_last_of("/\\");
		if (found != std::string::npos)
		{
			return projectDir.substr(0, found + 1);
		}
	}

	return "";
}

std::string ReadHLSLFile(const std::string fileName)
{
	std::string absolutePath = GetProjectDirectory() + "Shaders\\" + fileName;
	std::ifstream file(absolutePath);

	if (!file.is_open())
	{
		std::cerr << "Error opening file: " << absolutePath << "\n";
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	return buffer.str();
}