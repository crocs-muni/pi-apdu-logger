#include "Filter.h"

Filter::Filter() {
  fileManager = FileManager();
}

void Filter::setSampleSize(double ss)
{
  this -> ssize = ss;
}

int Filter::determineBitRepetitions(int sstream)
{
  return round((double)sstream/ssize);
}

void Filter::countBitRepetitions()
{
  char current_bit;
  int count = 0;
  std::pair<int,char> p;

  while(!fileStream.eof()) 
  { 
    fileStream.get(current_bit); // count the number of occurences of the same character
    count++;

    if(fileStream.eof()) break;

    if(fileStream.peek() != current_bit)
    {
      p.first = count;
      p.second = current_bit;
      ssizes.push_back(p);
      count = 0;
    }
  }   
  // remove the first and last sample counts because they are card init and disconnect
  ssizes.erase(ssizes.begin());
  ssizes.erase(ssizes.end());
}

std::pair<int,int> Filter::findLongestPauses()
{
  std::vector<int> pauses;
  for(auto it = ssizes.begin(); it < ssizes.end(); it++)
  {
    if(it -> second == '1')
    {
      pauses.push_back(it -> first);
    }
  }
  sort(pauses.begin(), pauses.end(), std::greater<int>());
  return std::make_pair(pauses[0], pauses[1]);
}

int Filter::filterData(int start_index)
{
  int i = start_index; // Start index for the ssizes vector
  while (i < ssizes.size())
  {
    int char_count = ssizes[i].first;
    char bit = ssizes[i].second;
    int repetitions=determineBitRepetitions(char_count); // How many 0 or 1 to write to character frame
    /*
      There cannot be more than 8 consecutive 1 in a character frame.
      Highest possible repetitions count for consecutive 1 bits is in the case of
      character frame S:0 D:01111111 P:1 followed by 11 for the pause, making in total 10 + 1 bit for the rounding error
    */
    if(bit == '1' && repetitions > 11)
    {   
      character.insert(character.end(), 11, bit);
      character.insert(character.end(), 1, '\n');
    }
    else 
    {
      character.insert(character.end(), repetitions, bit);
    }
    if(bit == '1' && char_count > ssize*11) // Stop after reaching a long pause
    {
      return i+1;
    }
    i++;
  }
  return 0;  
}

void Filter::filterWithVariableSS()
{
  std::cout << "DBG: Opening raw.txt for reading.."  <<std::endl;
  fileStream = fileManager.openFile(RAW, std::ios::in);
  countBitRepetitions();
  fileStream.close();
  std::pair<int,int> pauses = findLongestPauses();

  setSampleSize(ATR_SS);
  int index = 0;
  while(true)
  { 
    int next_index = filterData(index);
    if(next_index == 0)
    {
      break;
    }

    index = next_index;

    if(ssizes[index].first < ssize)
      setSampleSize(ssizes[index].first);

    if(ssizes[index - 1].first == pauses.first) //after reaching the longest pause sample with non_atr ss
      setSampleSize(NON_ATR_SS);

    if(ssizes[index - 1].first == pauses.second && round(ssizes[index].first/ATR_SS) == 1) //The ATR is sent one more time at the end of the communication
    { 
      setSampleSize(ATR_SS);
      filterData(index);
      break;
    }
  }
  std::cout << "DBG: Opening filtered.txt for writing.."  <<std::endl;
  fileStream = fileManager.openFile(FILTERED, std::ios::out);
  std::cout << "DBG: Writing to file filtered.txt.."  <<std::endl;
  fileManager.writeToFile(fileStream, character);
  fileStream.close();
  std::cout << "DBG: Finished!"  <<std::endl;
}