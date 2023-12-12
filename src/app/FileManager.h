#include <fstream>
#include <vector>

#define RAW "raw.txt"
#define FILTERED "filtered.txt"
#define DECODED "decoded.txt"

#ifndef FileManager_H
#define FileManager_H

class FileManager
{
  public:
  std::fstream openFile(std::string file_path, std::ios_base::openmode mode);
  std::vector<std::string> readFile(std::fstream &indata);
  void writeToFile(std::fstream &outdata, std::vector<char> data);
  void writeToFile(std::fstream &outdata, std::string data);
  void writeToFile(std::fstream &outdata, volatile int * buffer, int buffSize);
};

#endif
