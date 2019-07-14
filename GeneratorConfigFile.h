#pragma once
#include "FileReader.h"
#define RES 362
#define CROPRES 766

//this class represents a configuration file for the terraingenerator
class GeneratorConfigFile : public FileReader {
public:

	GeneratorConfigFile(const wchar_t* fileName);
	~GeneratorConfigFile();

	//retrieve information about the configuration of the terraingenerator
	int getResolution();
	float getCropResolution();
	
private:

	//parse the cfg file
	void ProcessFile() override;

	//config information
	int resolution;
	float cropResolution;

	//"hashes" incoming string
	int val(std::string str);
};

