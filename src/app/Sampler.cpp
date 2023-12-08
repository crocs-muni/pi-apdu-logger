#include "Sampler.h"

Sampler::Sampler()
{
  fileManager = FileManager();
}

void Sampler::sample()
{ 
	wiringPiSetupGpio(); // use BCM numbering
	pinMode(CARD_INPUT, INPUT);
	printf("DBG: Set pin %d as input.\n", CARD_INPUT);
	
	int wait;
	printf("DBG: Waiting for card initialization..\n");
	
	do 
	{
		wait = digitalRead(CARD_INPUT);
	} while(wait == 0); // Wait for the card to be in HIGH state
	
	//printf("DBG: Card initialized. Sampling..\n");
	for (int i=0; i<BUFF_SIZE; i++)
	{
		if(digitalRead(CARD_INPUT) == 0) // In case GPIO pin is connected to a npn transistor
			buffer[i] = 1;
		else
			buffer[i] = 0;
		delayMicroseconds(1);
	}
	printf("DBG: Sampling finished.\n");
	
	printf("DBG: Opening raw.txt for writing..\n");
	fileStream = fileManager.openFile(RAW, std::ios_base::out);
	
	printf("DBG: Writing to file raw.txt..\n");
	fileManager.writeToFile(fileStream, buffer, BUFF_SIZE);
	printf("DBG: Finished.\n");
	fileStream.close();
}
