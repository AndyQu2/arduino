/* 
	Ideas and write by AndyQu (Austin)
 	MateROV2025 Competition
  	Task1.3 - eDNA of invasive carp
   	Determine if the unknown sequences include invasive carp
*/
#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
using namespace std;

struct DNA_SAMPLE_STRUCT {
	string head;
	string tail;
	string body;
};

DNA_SAMPLE_STRUCT bigheadCarp;
DNA_SAMPLE_STRUCT silverCarp;
DNA_SAMPLE_STRUCT grassCarp;
DNA_SAMPLE_STRUCT blackCarp;

vector<string> dna_sample_filepath = {"dna-sample/BHC.txt", "dna-sample/SC.txt", "dna-sample/GC.txt", "dna-sample/BC.txt"};
const string dna_given_filepath = "dna_given.txt";
vector<string> dna_given;

string trim(const string& str) {
	size_t first = str.find_first_not_of(' ');
	if (first == string::npos) {
		return "";
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

int readDNASampleFromFile(string fileName, DNA_SAMPLE_STRUCT & data_struct) {
	ifstream file(fileName);
	string data;
	
	if (!file.is_open()) {
		cerr << "Error occur in opening the file!" << endl;                             
		return 1;
	}
	getline(file, data_struct.head);
	getline(file, data_struct.tail);
	getline(file, data_struct.body);
	
	file.close();
	return 0;
}

vector<string> readDNAGivenFromFile() {
	vector<string> dataGroups;
	ifstream file(dna_given_filepath);
	if (!file.is_open()) {
		cerr << "Errors occurs in opening file!" << endl;
		return dataGroups;
	}
	
	string line;
	string currentGroups;
	bool capturing = false;
	regex sampleRegex("Unknown Sample \\d+");
	
	while(getline(file, line)) {
		line = trim(line);
		
		if(regex_search(line, sampleRegex)) {
			if (capturing && !currentGroups.empty()) {
				dataGroups.push_back(currentGroups);
				currentGroups.clear();
			}
			capturing = true;
			continue;
		}
		
		if (capturing) {
			if (line.empty()) {
				continue;
			}
			currentGroups += line;
		}
	}
	
	if (capturing && !currentGroups.empty()) {
		dataGroups.push_back(currentGroups);
	}
	
	file.close();
	return dataGroups;
}

int compareDNA(DNA_SAMPLE_STRUCT sample, string dna) {
	int flagHead = dna.find(sample.head);
	int flagTail;
	if (flagHead) flagTail = dna.find(sample.tail);
	int flagBody;
	
	if ((flagHead != string::npos) && (flagTail != string::npos)) {
		flagBody = dna.find(sample.body);
		
		if ((flagHead + sample.head.size() == flagBody) && (flagBody + sample.body.size() == flagTail)) {
			return 1;
		}
	}
	return 0;
}

int main() {
	readDNASampleFromFile(dna_sample_filepath[0], bigheadCarp);
	readDNASampleFromFile(dna_sample_filepath[1], silverCarp);
	readDNASampleFromFile(dna_sample_filepath[2], grassCarp);
	readDNASampleFromFile(dna_sample_filepath[3], blackCarp);
	
	dna_given = readDNAGivenFromFile();
	
	for (int i = 0; i < dna_given.size(); i ++) {	
		bool isBighead = compareDNA(bigheadCarp, dna_given[i]);
		bool isSilver = compareDNA(silverCarp, dna_given[i]);
		bool isGrass = compareDNA(grassCarp, dna_given[i]);
		bool isBlack = compareDNA(blackCarp, dna_given[i]);
		
		if (isBighead) cout << "Unknown Sample " << i + 1 << " matches that of the Bighead carp" << endl;
		if (isSilver) cout << "Unknown Sample " << i + 1 << " matches that of the Silver carp" << endl;
		if (isGrass) cout << "Unknown Sample " << i + 1 << " matches that of the Grass carp" << endl;
		if (isBlack) cout << "Unknown Sample " << i + 1 << " matches that of the Black carp" << endl;
	}
	return 0;
}
