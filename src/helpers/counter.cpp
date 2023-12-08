#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/* 
    This is a helper module for the APDULogger
    It prints a histogram of possible sample sizes for the Filter module
*/

int main(int argc, char **argv) 
{
	char filename[20];
  ifstream indata;
    
	cout << "Enter raw data file name: ";
	cin >> filename;
    
	cout << "Opening file " << filename << "..." << endl;
	
  indata.open(filename);
	if(!indata)
	{
		cerr << "Error: file to read from could not be opened!" << endl;
		exit(1);
	}

	char current_bit;
	int char_position = 0;
	int sample = 0;
	vector<int> frequencies;
	vector<int> histogram(100, 0);
	
	
  cout << "Computing histogram... " << endl;
  while(!indata.eof()) 
	{
		indata.get(current_bit);
		sample++;
		if(indata.eof()) break;

    if(indata.peek() != current_bit)
		{
			frequencies.push_back(sample); //push back the current frequency and reset the counter
			sample = 0;
    }
	}
	indata.close();
	
	for (auto it = frequencies.begin(); it != frequencies.end(); it++)
	{
		if(*it <= 100) histogram[*it]++; //compute a histogram of possible sample sizes
	}

	cout << endl << "-----------------Simple histogram-----------------" << endl;
	for (int i=1; i<histogram.size(); i++)
	{
		cout << "Sample size " << i << " observed " << histogram[i] << " time(s)." <<endl;
	}
    
	return 0;
}