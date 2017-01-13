
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include <Windows.h>
#include <process.h>

#include "Types.h"

namespace CPlusPlusLogging
{
	#define LOG_ERROR(x)	Logger::getInstance()->error(x)
	#define LOG_ALARM(x)	Logger::getInstance()->alarm(x)
	#define LOG_ALWAYS(x)	Logger::getInstance()->always(x)
	#define LOG_INFO(x)		Logger::getInstance()->info(x)
	#define LOG_BUFFER(x)	Logger::getInstance()->buffer(x)
	#define LOG_TRACE(x)	Logger::getInstance()->trace(x)
	#define LOG_DEBUG(x)	Logger::getInstance()->debug(x)

   class Logger
   {
	private:
		static Logger*          m_Instance;
		std::ofstream           m_File;
		CRITICAL_SECTION        m_Mutex;
		LogLevel                m_LogLevel;
		LogType                 m_LogType;

	protected:
		Logger();
		~Logger();
		void lock();
		void unlock();
		std::string getCurrentTime();

	private:
		void logIntoFile(std::string& data);
		void logOnConsole(std::string& data);
		Logger(const Logger& obj) {}
		void operator=(const Logger& obj) {}

	public:
        static Logger* getInstance() throw ();
        void error(const char* text) throw();
        void error(std::string& text) throw();
        void error(std::ostringstream& stream) throw();
        void alarm(const char* text) throw();
        void alarm(std::string& text) throw();
        void alarm(std::ostringstream& stream) throw();
        void always(const char* text) throw();
        void always(std::string& text) throw();
        void always(std::ostringstream& stream) throw();
        void buffer(const char* text) throw();
        void buffer(std::string& text) throw();
        void buffer(std::ostringstream& stream) throw();
        void info(const char* text) throw();
        void info(std::string& text) throw();
        void info(std::ostringstream& stream) throw();
        void trace(const char* text) throw();
        void trace(std::string& text) throw();
        void trace(std::ostringstream& stream) throw(); 
        void debug(const char* text) throw();
        void debug(std::string& text) throw();
        void debug(std::ostringstream& stream) throw();
        void updateLogLevel(LogLevel logLevel);
        void enaleLog(); 
        void disableLog(); 
        void updateLogType(LogType logType);
        void enableConsoleLogging();
        void enableFileLogging();
   };
}

#endif 

