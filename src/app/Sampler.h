#include <iostream>
#include <vector>
#include <fstream>
#include <wiringPi.h>
#include "FileManager.h"

#define CARD_INPUT 26
#define BUFF_SIZE 1000000

#ifndef Sampler_H
#define Sampler_H

class Sampler
{
	volatile int buffer[BUFF_SIZE];
	std::fstream fileStream;
	FileManager fileManager;

	public:
	Sampler();
	void sample();
};

#endif
