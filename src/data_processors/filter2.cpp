#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;

/* Filtering raw_data with sample size 4 again because for the 
	ATR  and PTS there are 26 samples per single bit,
	whereas for the rest of the communication there
	are approximately 6.5 samples per single bit.
 */
#define SAMPLE_SIZE 4 //determined by counter.cpp


int convertToInt(vector<char> value) {
	return round((double)value.size() / SAMPLE_SIZE);
}

void filter(ifstream &indata, ofstream &outdata) {
	char current_bit;
	int char_count = 0;
	vector<char> sample;
	bool start = true;
    bool atr = true;

	while(!indata.eof()) 
	{
		/*skip the initial ones*/
		while(start) {
			indata.get(current_bit);
			if(indata.peek() != current_bit) start = false;
		}
		
		/*read one character from input file*/
		indata.get(current_bit);
        char_count++;
		sample.push_back(current_bit);
        /*this is to ensure that filtering stops after reading the atr
        since the pause between the atr and the rest of the communication is around 3500 samples */
        if(char_count == 3200) { 
            atr = false;
        }
		
		if(indata.eof()) break;
		
		if(indata.peek() != current_bit && atr) {
			int res = convertToInt(sample);
            char_count = 0;
			sample.clear();
			for(int i = 0; i < res; i ++) {
				outdata << current_bit;
			}
		}
        if(!atr) {
            outdata << current_bit;
        }
	}
}

int main(int argc, char **argv) 
{
	printf("INFO: Running atr_filter.exe\n");
    
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
