#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include "FileManager.h"

//counter module is used to compute approximate values for sample sizes
#define ATR_SS 98.5
#define NON_ATR_SS 19.5


#ifndef Filter_H
#define Filter_H

class Filter 
{
  double ssize;
  std::vector<std::pair<int,char>> ssizes;
  std::vector<char> character;
  std::fstream fileStream;
  FileManager fileManager;

  public:
  Filter();
  void setSampleSize(double ss);
  int determineBitRepetitions(int sstream);
  void countBitRepetitions();
  std::pair<int,int> findLongestPauses();
  int filterData(int start_index);
  void filterWithVariableSS();
};

#endif
