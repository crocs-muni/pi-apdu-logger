#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Sampler.h"
#include "Filter.h"
#include "Decoder.h"
#include "FileManager.h"

/*
* g++ -o ./executables/apduLogger ./source/Sampler.cpp ./source/Filter.cpp ./source/FileManager.cpp ./source/Decoder.cpp ./source/APDULogger.cpp -l wiringPi
* */

class APDU 
{
  std::string header;
  std::string body;
  int dataLen;

  public:
  APDU(std:: string header, std::string body, int dataLen)
  {
    this -> header = header;
    this -> body = body;
    this -> dataLen = dataLen;
  }
  std::string getHeader()
  {
    return this -> header;
  }
  std::string getBody()
  {
    return this -> body;
  }
  int getDataLen()
  {
    return this -> dataLen;
  }
  void setHeader(std::string header)
  {
    this -> header = header;
  }
  void setBody(std::string body)
  {
    this -> body = body;
  }
  void setDataLen(int dataLen)
  {
    this -> dataLen = dataLen;
  }
  void printToConsole()
  {
    std::cout << "APDU: " << this -> header;
    if(!this -> body.empty())
      std::cout << " data length: " << this -> dataLen/2 << " byte(s) data: " << this -> body;
    std::cout << std::endl;
  }
};

class BlockFrame
{
  std::string prologue;
  std::string infField;
  std::string epilogue;
  int length;

  public:
  BlockFrame(std::string prologue, int length, std::string infField, std::string epilogue)
  {
    this -> prologue = prologue;
    this -> length = length;
    this -> infField = infField;
    this -> epilogue = epilogue;
  }
  int getLength()
  {
    return length;
  }
  std::string getInfField()
  {
    return infField;
  }
  std::string getEpilogue()
  {
    return epilogue;
  }
  void printToConsole()
  {
    std::cout << "P: " << prologue << " I: " << infField << " E: " << epilogue << std::endl;
  }
};


class Logger 
{
  std::string atr;
  std::string cardEnd;
  std::string endAtr;
  std::vector<BlockFrame> frames;
  std::vector<APDU> apdus;

  public:
  void setATR(std::string atr)
  {
    this -> atr = atr.erase(atr.size() - 2);
  }
    void setCardEnd(std::string cardEnd)
  {
    this -> cardEnd = cardEnd;
  }
    void setEndATR(std::string endAtr)
  {
    this -> endAtr = endAtr.erase(endAtr.size() - 2);
  }

  std::string getATR()
  {
    return atr;
  }

  void updateAPDUs()
  {
    for (int i = 0; i < frames.size(); i++)
    {
      if(i%2 == 0)
      {
        addCommandAPDU(frames[i].getInfField());
      } else
      {
        addResponseAPDU(frames[i].getInfField(), frames[i].getLength());
      }
    }
  }
  void updateBlockFrames(std::vector<std::string> fileContent)
  {
    for (int i=5; i<fileContent.size()-1; i++) //skip atr + pts
    { 
      std::string line = fileContent[i];

      std::string prologue = line.substr(0, 6);
      int prologLen = std::stoi(line.substr(4, 2), 0, 16)*2; //multiplying by 2 because the hex representations uses 2 chars
      std::string infField = line.substr(6, prologLen);
      std::string epilogue = line.substr(line.size() - 5, 4);

      frames.push_back(BlockFrame(prologue, prologLen, infField, epilogue));
    }
  }
  void addCommandAPDU(std::string infField)
  {
    std::string header = infField.substr(0, 8);
    int dataLen = std::stoi(infField.substr(8, 2), 0, 16)*2;
    std::string body = infField.substr(10, dataLen);

    apdus.push_back(APDU(header, body, dataLen));
  }

  void addResponseAPDU(std::string infField, int prologueLen)
  {
    int dataLen = prologueLen - 4;
    std::string header = infField.substr(0,4);
    std::string body;

    if(dataLen != 0)
    {
      header = infField.substr(dataLen, 4);
      body = infField.substr(0, dataLen);
    }
    apdus.push_back(APDU(header, body, dataLen));
  }

  void printLog()
  {
    std::cout << "# ATR: " <<  atr <<std::endl;
    for(APDU apdu:apdus)
    {
      apdu.printToConsole();
    } 
    std::cout << "# END: " <<  endAtr <<std::endl;
  }
};

int main(int argc, char **argv) 
{
  printf("# APDU Logger\n");

  Sampler sampler = Sampler();
  Filter filter = Filter();
  Decoder decoder = Decoder();

  sampler.sample();
  filter.filterWithVariableSS();
  decoder.decode();

  FileManager fileManager = FileManager();
  std::fstream indata = fileManager.openFile(DECODED, std::ios::in);
  std::vector<std::string> fileContent = fileManager.readFile(indata);

  Logger logger = Logger();
  logger.setATR(fileContent[0]);
  logger.setEndATR(fileContent[fileContent.size() - 1]);
  logger.updateBlockFrames(fileContent);
  logger.updateAPDUs();
  logger.printLog();

  return 0;
}
