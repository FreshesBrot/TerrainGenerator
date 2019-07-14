#include "GeneratorConfigFile.h"

GeneratorConfigFile::GeneratorConfigFile(const wchar_t* fileName) : FileReader::FileReader(fileName) {
}

GeneratorConfigFile::~GeneratorConfigFile() {
}

void GeneratorConfigFile::ProcessFile() {
	std::string newLine;
	while ((newLine = nextLine()).compare(END_OF_FILE) != 0) {

	}
}
