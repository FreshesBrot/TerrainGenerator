#include "GeneratorConfigFile.h"

GeneratorConfigFile::GeneratorConfigFile(const wchar_t* fileName) : FileReader::FileReader(fileName) {
	resolution = 0;
	cropResolution = 0;
	sigma = 0;
	dir = "";
	mapFileName = "";
	nMapFileName = "";
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

float GeneratorConfigFile::getFallOff() {
	return fallOff;
}

int GeneratorConfigFile::getIterations() {
	return iterations;
}

float GeneratorConfigFile::getSigma() {
	return sigma;
}

std::string GeneratorConfigFile::getMapFileName() {
	return std::string(dir+mapFileName);
}

std::string GeneratorConfigFile::getNMapFileName() {
	return std::string(dir + nMapFileName);
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
			if (resolution <= 0) resolution = 512;
			break;
		}
		case CROPRES:
		{
			newLine = nextLine();
			std::istringstream iss(newLine);
			iss >> cropResolution;
			if (cropResolution <= 0 || cropResolution >= 1) cropResolution = 0.5;
			break;
		}
		case FALLOFF: 
		{
			newLine = nextLine();
			std::istringstream iss(newLine);
			iss >> fallOff;
			break;
			if (fallOff > 1 || fallOff < 0) fallOff = 0.8;
		}
		case DIR:
		{
			dir = nextLine();
			break;
		}
		case MAPNAME:
		{
			mapFileName = nextLine();
			break;
		}
		case NMAPNAME:
		{
			nMapFileName = nextLine();
			break;
		}
		case SMOOTH:
		{
			newLine = nextLine();
			std::istringstream iss(newLine);
			iss >> iterations;
			if (iterations < 1) iterations = 1;
			break;
		}
		case SIGMA:
		{
			newLine = nextLine();
			std::istringstream iss(newLine);
			iss >> sigma;
			if (sigma <= 0) sigma = 0.5;
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
