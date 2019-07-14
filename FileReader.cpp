#include "FileReader.h"


FileReader::FileReader(const wchar_t* fileName) : fs() {
	this->fileName = fileName;
	file = toString(std::wstring(fileName));
}

FileReader::~FileReader() { 
	if (fs.is_open())
		fs.close();
}

void FileReader::Read() { 
	fs.open(fileName); 
}

std::string FileReader::nextLine() {
	std::string line;
	this->line++;
	if (std::getline(fs, line))
		return line;
	return std::string(END_OF_FILE);
}

void FileReader::Close() {
	if (fs.is_open())
	fs.close();
}

std::string FileReader::toString(std::wstring ws) {
	return std::string(ws.begin(), ws.end());
}

const wchar_t* FileReader::toWString(std::string s) {
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, wstr, wchars_num);
	return wstr;
}
