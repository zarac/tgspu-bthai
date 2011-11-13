#ifndef __LOGGER_H__
#define __LOGGER_H__

//#include <fstream>
//using std::ofstream;

#include <string>
using std::string;

class Logger
{
private:
	Logger();
	static Logger* instance;
	static bool gotInstance;
//	static ofstream out;
public:
	~Logger();
	static Logger* getInstance();
	static void Log(char[]);
	static void Log(string);
	void log(char[]);
	void log(string);
};

#endif