#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;

/* Filtering raw_data with sample size 6.5 and rounding either up or down, 
 * depending on the value obtained 
 */

#define SAMPLE_SIZE 6.5 //determined by counter.cpp


int convertToInt(vector<char> value) {
	return round((double)value.size() / SAMPLE_SIZE);
}

void filter(ifstream &indata, ofstream &outdata) {
	char current_bit;
	int char_count = 0;
	vector<char> sample;
	bool start = true;
	
	while(!indata.eof()) 
	{
		/*skip the initial zeros*/
		while(start) {
			indata.get(current_bit);
			if(indata.peek() != current_bit) start = false;
		}
		
		/*read one character from input file*/
		indata.get(current_bit);
		// char_count ++;
		sample.push_back(current_bit);
		
		if(indata.eof()) break;
		
		if(indata.peek() != current_bit) {
			int res = convertToInt(sample);
			sample.clear();
			for(int i = 0; i < res; i ++) {
				outdata << current_bit;
			}
			// if(char_count == 80) {
			// 	outdata << '\n';
			// 	char_count = 0;
			// }
		}
	}
}

int main(int argc, char **argv) 
{
    printf("INFO: Running filter.exe\n");
    
    if(argc !=3) {
		cerr << "Error: Invalid number of arguments!" << endl;
		exit(1);
	}
	
	ifstream indata;
	ofstream outdata;
	string inputfile = argv[1]; //first argument is input file name
	string outputfile = argv[2]; //second argument is output file name
	
	printf("Open reading file...\n");
	indata.open(inputfile);
    
    if(!indata) {
		cerr << "Error: file to read from could not be opened!" << endl;
		exit(1);
	}

	printf("Open writing file...\n");
	outdata.open(outputfile);
	
	if(!outdata) {
		cerr << "Error: file to write to could not be opened!" << endl;
		exit(1);
	}
	
	printf("Filtering data...\n");
	filter(indata, outdata);
	
	indata.close();
	outdata.close();
	cout << "Finish filtering data." << endl;
	
	return 0;
}
