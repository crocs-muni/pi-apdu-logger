#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define DATA_SIZE 10 /* 10 bits per data block */


void read(ifstream &in, string &string) {
    int position = 0;
    while(position <= DATA_SIZE)
    {
        in >> string[position];
        ++position;
    }
}

void reverseString(string& string, int end, int begin) {
	if(end <= begin) return;
    swap(string[begin],string[end]);
	reverseString(string,end-1, begin+1);
}

string convertToHex(string binary) {
	/* LSB is the first bit, MSB is the last bit in the original byte*/
	reverseString(binary, binary.length()-2, 1);
	/* discard 1st - start and 10th - parity bit */
	int result = 0;
	for(int i = 1; i < binary.length() - 1; i++) { 
		result *= 2;
		result += binary[i] == '1' ? 1 : 0;
	}
	stringstream stream;
	stream << hex << result;
	return stream.str();
}

void decode(ifstream &indata, ofstream &outdata) {
	string current_bit;
	current_bit.resize(DATA_SIZE); 
	int char_position = 0;
	int number_of_ones = 0;
    bool pause = false;

	while(!indata.eof()) 
	{
		read(indata, current_bit);
		string res = convertToHex(current_bit);
		/* This is to ensure that every hex value is represented by 2 characters
		for example 0 is 00, 2 is 02 and so on */
		if(res.length() == 1) outdata << '0';
		outdata << res;
		
		if(indata.eof()) break;
		
        /* Move the cursor to read the next bit */
        char_position += DATA_SIZE;
        /* Save the new position in the data stream */
        indata.seekg(char_position);
        number_of_ones = 0;
        pause = false;
		/* There is a short pause between successive atr data blocks*/
		while(indata.peek() != '0') {
			if(indata.eof()) break;
			indata.seekg(++char_position);
			number_of_ones++;
             /*pause between two successive bytes is equal to 2 samples of logic level 1
             more than 2 samples mean that another APDU is being sent*/
            if(number_of_ones > 5 && !pause) {
				outdata << '\n';
                pause = true;
			}
		} 
	}
}

int main(int argc, char **argv) 
{
    printf("INFO: Running decoder.exe\n");
    
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
	
	printf("Decoding data...\n");
	decode(indata, outdata);
	
	indata.close();
	outdata.close();
	cout << "Finish decoding data." <<endl;

	return 0;
}
