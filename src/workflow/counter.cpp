#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/*  This is not the optimal version for finding maximum elements in a vector,
    but since the vector size is quite small, it is fast enough.
    For larger vectors, the algorithm should be optimized.
*/

int find_max_element_index(vector<int> histogram) {
   int max_el = histogram[0];
   int max_el_index = 0;

   for(int i=0; i<histogram.size(); i++) {
        if(histogram[i] > max_el) {
            max_el = histogram[i];
            max_el_index = i;
        }
   }
   return max_el_index;
}

int main(int argc, char **argv) 
{
	char filename[20];
    ifstream indata;
    
    cout << "Enter raw data file name: ";
    cin >> filename;
    
	cout << "Opening file " << filename << "..." << endl;
	
    indata.open(filename);
	if(!indata) {
		cerr << "Error: file to read from could not be opened!" << endl;
		exit(1);
	}

	char current_bit;
	int char_position = 0;
	int sample = 0;
	vector<int> frequencies;
	vector<int> histogram(100, 0);
    vector<int> max_values; /* this vector holds communication data and atr sample size*/
	
	while(!indata.eof()) 
	{
		/*read one character from input file*/
		indata.get(current_bit);
        sample++;
        if(indata.eof()) break;

        if(indata.peek() != current_bit) {
            /*push back the current frequency and reset the counter*/
			frequencies.push_back(sample);
			sample = 0;
        }
	}
	indata.close();
	
    for (auto it = frequencies.begin(); it != frequencies.end(); it++) {
        if(*it <= 100) histogram[*it]++; /* compute a histogram of possible sample sizes*/
	}

	vector<int> histogram_copy = histogram;
    
    for (int i = 0; i < 4; i++) {
		int index = find_max_element_index(histogram_copy);
        max_values.push_back(index);
        histogram_copy[index] = 0; 
	}

    cout << "Sample sizes: " << endl;
    if(max_values[1] + max_values[2] == max_values[0]) {
        double filter1_sample_size = (double) max_values[0]/2;
        cout << "For filter1.exe sample size is " << filter1_sample_size << "." << endl;
        cout << "For filter2.exe sample size is " << round((double)max_values[3]/filter1_sample_size) << "." << endl;
    }
    else {
        cout << "Sample size could not be determined." << endl;
        exit(1);
    }
    
	cout << "-----------------Simple histogram-----------------" << endl;
    for (int i = 0; i < histogram.size(); i++) {
		cout << "index " << i <<": "<< histogram[i] << endl;
	}
	
	return 0;
}
