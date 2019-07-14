#pragma once
#include "FileReader.h"


//this class represents a configuration file for the terraingenerator
class GeneratorConfigFile : public FileReader {
public:

	GeneratorConfigFile(const wchar_t* fileName);
	~GeneratorConfigFile();

	//retrieve information about the configuration of the terraingenerator

	
private:

	//parse the cfg file
	void ProcessFile() override;


};

