#pragma once
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <Windows.h>
//#include "FileReaderException.h"
#define END_OF_FILE "0"


//this class represents an object that can read a file
class FileReader {
public:
		
	FileReader(const wchar_t* fileName);
	~FileReader();

	//initiates the file stream
	void Read();

	//returns the next line of the file
	//returns null if file is at the end
	std::string nextLine();
	
	//closes the file stream
	void Close();

protected:

	//conversion from wchar to char (vice versa)
	std::string toString(std::wstring ws);
	const wchar_t* toWString(std::string s);
	

	//the file stream the class operates on
	std::fstream fs;

	//name of the file
	std::wstring fileName;
	//storage for error messages
	std::string file;
	int line = 0;

	//processes the file
	virtual void ProcessFile() = 0;

};

