#include "GeneratorConfigFile.h"

GeneratorConfigFile::GeneratorConfigFile(const wchar_t* fileName) : FileReader::FileReader(fileName) {
	resolution = 0;
	cropResolution = 0;
	ProcessFile();
}

GeneratorConfigFile::~GeneratorConfigFile() {
}

int GeneratorConfigFile::getResolution() {
	return resolution;
}

float GeneratorConfigFile::getCropResolution() {
	return cropResolution;
}

void GeneratorConfigFile::ProcessFile() {
	//open file
	Read();
	
	std::string newLine;
	//capture next string
	while ((newLine = nextLine()).compare(END_OF_FILE) != 0) {
		int c;
		switch (c = val(newLine)) {
		case RES: 
		{
			newLine = nextLine();
			std::istringstream iss(newLine);
			iss >> resolution;
			break;
		}
		case CROPRES:
		{
			newLine = nextLine();
			std::istringstream iss(newLine);
			iss >> cropResolution;
			break;
		}
		default: break;
		}
	}
}

int GeneratorConfigFile::val(std::string str) {
	int sum = 0;
	std::string::iterator it;
	for (it = str.begin(); it < str.end(); it++) {
		sum += *it;
	}
	return sum;
}
