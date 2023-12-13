#include "Decoder.h"


class UnknownCoding : public std::exception
{
public:
  virtual const char* what() const throw() 
  { 
    return "ERROR: Cannot determine coding convention!"; 
  }      
};

Decoder::Decoder()
{
  fileManager = FileManager();
}

void Decoder::setConvention(coding convention)
{
  this -> convention = convention;
}

coding Decoder::getConvention()
{
  return this -> convention;
}

void Decoder::determineCodingConvention(std::string byte)
{
  if(byte == "0110111001\0")
    setConvention(direct);
  else if(byte == "0110000001\0")
    setConvention(inverse);
  else
    throw UnknownCoding();
}

void Decoder::reverse(std::string& string, int end, int begin)
{
	if(end <= begin) return;
  std::swap(string[begin],string[end]);
	reverse(string,end-1, begin+1);
}

std::string Decoder::convertToHex(std::string binary)
{
  if (getConvention() == direct)
  {
    //LSB is the first bit, MSB is the last bit in the original byte
    reverse(binary, binary.length() - 2, 1);
  }
  //discard 1st - start and 10th - parity bit
  int result = 0;
  for(int i = 1; i < binary.length() - 1; i++)
  { 
    result *= 2;
    result += binary[i] == '1' ? 1 : 0;
  }
  std::stringstream stream;
  stream << std::hex << result;
  return stream.str();
}

void Decoder::processByte(std::string data)
{
  int pos = 0;
  bool pause = false;

  while(pos < data.length())
  {
    if(pause && data[pos] != '0')
    {
      //std::cout << "DBG: Extra pause bit at position: " << pos << std::endl; 
      pos--;
      pause = false;
    }
    std::string byte_str = data.substr(pos, 10);
    std::string hex_stream = convertToHex(byte_str);

    if(hex_stream.length() == 1) hexDump += '0';
    hexDump += hex_stream;

    pos += 10;

    if(data[pos] == '1')
    {
      pos++;
      if(data[pos] == '1')
      {
        pos++;
        pause = true;
      } else
      {
        // std::cout << "DBG: Missing pause bit at position: " << pos << std::endl;
        pause = false;
      }
    }
  }
}

void Decoder::decode()
{
  std::cout << "DBG: Opening filtered.txt for reading.."  <<std::endl;
  fileStream = fileManager.openFile(FILTERED, std::ios::in);
  characters = fileManager.readFile(fileStream);
  fileStream.close();

  try
  { 
    determineCodingConvention(characters[0].substr(0, 10));
  } 
  catch (UnknownCoding ex)
  {
    std::cerr << ex.what() << std::endl;
    exit(1);
  }

  for (int i = 0; i < characters.size(); i++)
  {
    processByte(characters[i]);
    hexDump += '\n';
  }
  std::cout << "DBG: Opening decoded.txt for writing.."  <<std::endl;
  fileStream = fileManager.openFile(DECODED, std::ios::out);
  std::cout << "DBG: Writing to file decoded.txt.."  <<std::endl;
  fileManager.writeToFile(fileStream, hexDump);
  fileStream.close();
  std::cout << "DBG: Finished!"  <<std::endl;
}