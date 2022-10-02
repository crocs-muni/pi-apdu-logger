#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

#define INPUT_FILE "raw_data.txt"


int main(int argc, char **argv) 
{
	ifstream indata;
	printf("Open reading file...\n");
	indata.open(INPUT_FILE);
	if(!indata) {
		cerr << "Error: file to read from could not be opened!" << endl;
		exit(1);
	}
	char current_bit;
	int char_position = 0;
	int zeros = 0;
	int ones = 0;
	vector<int> frequencies;
	vector<int> histogram(100, 0);
	int ones_freq = 0;
	int zeros_freq = 0;
	
	printf("Filtering data...\n");
	while(!indata.eof()) 
	{
		/*read one character from input file*/
		indata.get(current_bit);
		if(current_bit == '1') ones++;
		else zeros++;
		
		if(indata.eof()) break;
		
       
		if(indata.peek() == '1' && current_bit == '0') {
			/*push back the current frequency and reset the counter*/
			frequencies.push_back(ones);
			ones = 0;
		
		}
		if(indata.peek() == '0' && current_bit == '1') {
			/*push back the current frequency and reset the counter*/
			frequencies.push_back(zeros);
			zeros = 0;
		}
	}
	indata.close();
	for (auto it = frequencies.begin(); it != frequencies.end(); it++) {
		if(*it > 100) cout << "Frequencies:" << *it <<endl;
		else histogram[*it]++;
	}
	
	
	for (int i =0; i < histogram.size(); i++) {
		cout << "index " << i <<": "<<histogram[i] <<endl;
	}

	auto it = max_element(histogram.begin(), histogram.end());
	cout << "Maximum frequency is: "<< *it <<endl;
	
	return 0;
}
