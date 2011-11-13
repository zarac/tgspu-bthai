#include "Logger.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <fstream>
using std::ofstream;

//#include <cstdlib>

bool Logger::gotInstance = false;
Logger* Logger::instance = NULL;
ofstream out;

Logger::Logger()
{
	//ofstream out("blah.log");
	out.open("sl00tbot.log");
	if (!out)
	{
		cerr << "Error: file could not be opened." << endl;
		exit(1);
	}
	else
		out << "Log file opened." << endl;
}

Logger::~Logger()
{
	out << "Closing log file." << endl;
	out.close();
}

Logger* Logger::getInstance()
{
	if (gotInstance == false)
	{
		Logger::instance = new Logger();
		Logger::gotInstance = true;
	}
	return Logger::instance;
}

void Logger::Log(char str[])
{
	getInstance()->log(str);
}

void Logger::log(char str[])
{
	out << str << endl;
}

void Logger::Log(string str)
{
	getInstance()->log(str);
}

void Logger::log(string str)
{
	out << str << endl;
}