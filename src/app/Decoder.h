#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "FileManager.h"

#ifndef Decoder_H
#define Decoder_H

enum coding {
    direct,
    inverse
};

class Decoder 
{
    coding convention;
    std::vector<std::string> characters;
    std::string hexDump;
    std::fstream fileStream;
    FileManager fileManager;

    public:
    Decoder();
    void setConvention(coding convention);
    coding getConvention();
    void reverse(std::string& string, int end, int begin);
    std::string convertToHex(std::string binary);
    void determineCodingConvention(std::string byte);
    void processByte(std::string data);
    void decode();
};

#endif