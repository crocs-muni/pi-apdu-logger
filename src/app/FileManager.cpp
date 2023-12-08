#include <iostream>
#include <string>

#include "FileManager.h"

std::fstream FileManager::openFile(std::string file_path, std::ios_base::openmode mode)
{
	std::fstream file;
	
	file.open(file_path, mode);
	if (!file.is_open()) 
	{
		std::cerr << "Unable to open file: " << file_path << std::endl;
		exit(1);
	}
	return file;
}

std::vector<std::string> FileManager::readFile(std::fstream &indata)
{
	std::vector<std::string> lines;
	std::string line;

	while (std::getline(indata, line))
	{ 
		lines.push_back(line);
	}
	return lines;
}

void FileManager::writeToFile(std::fstream &outdata, std::vector<char> data)
{
	for(auto it=data.begin(); it<data.end(); it++)
	{
		outdata << *it;
	}
}

void FileManager::writeToFile(std::fstream &outdata, std::string data)
{
	for(auto it=data.begin(); it<data.end(); it++)
	{
		outdata << *it;
	}
}

void FileManager::writeToFile(std::fstream &outdata, volatile int * buffer, int buffSize)
{
	for(int i=0; i<buffSize; i++)
	{
		outdata << buffer[i];
	}
}
