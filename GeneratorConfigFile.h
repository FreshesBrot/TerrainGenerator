#pragma once
#include "FileReader.h"
#define RES 330
#define CROPRES 734
#define FALLOFF 698
#define DIR 631
#define MAPNAME 703
#define NMAPNAME 781
#define SMOOTH 984

//this class represents a configuration file for the terraingenerator
class GeneratorConfigFile : public FileReader {
public:

	GeneratorConfigFile(const wchar_t* fileName);
	~GeneratorConfigFile();

	//retrieve information about the configuration of the terraingenerator
	int getResolution();
	float getCropResolution();
	float getFallOff();
	int getIterations();

	//retrieve information about the storage
	std::string getMapFileName();
	std::string getNMapFileName();


private:

	//parse the cfg file
	void ProcessFile() override;

	//config information
	int resolution;
	float cropResolution;
	float fallOff;
	int iterations;

	//storage information
	std::string dir;
	std::string mapFileName;
	std::string nMapFileName;

	//"hashes" incoming string
	int val(std::string str);
};

