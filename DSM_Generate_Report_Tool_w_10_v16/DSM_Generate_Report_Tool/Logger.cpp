#include "stdafx.h"
#include "Logger.h"
using namespace CPlusPlusLogging;

Logger* Logger::m_Instance = 0;

Logger::Logger()
{
	time_t lRawTime = 0;
	time (&lRawTime);
	struct tm* tagTimeInfo;
	tagTimeInfo = localtime(&lRawTime);
	char pLogFileName[256];
	sprintf(pLogFileName,"Log_%d_%d_%d.log",tagTimeInfo->tm_year+1900,tagTimeInfo->tm_mon+1,tagTimeInfo->tm_mday);
	m_File.open(pLogFileName, ios::out|ios::app);

	m_LogLevel	= LOG_LEVEL_TRACE;
	m_LogType	= FILE_LOG;

	InitializeCriticalSection(&m_Mutex);

}

Logger::~Logger()
{
	m_File.close();
	DeleteCriticalSection(&m_Mutex);
}

Logger* Logger::getInstance() throw ()
{
	if (m_Instance == 0) 
		m_Instance = new Logger();

	return m_Instance;
}

void Logger::lock()
{
	EnterCriticalSection(&m_Mutex);
}

void Logger::unlock()
{
	LeaveCriticalSection(&m_Mutex);
}

void Logger::logIntoFile(std::string& data)
{
	lock();
	m_File << getCurrentTime() << "  " << data << endl;
	unlock();
}

void Logger::logOnConsole(std::string& data)
{
	cout << getCurrentTime() << "  " << data << endl;
}

string Logger::getCurrentTime()
{
	string currTime;
	time_t now = time(0); 
	currTime.assign(ctime(&now));
	string currentTime = currTime.substr(0, currTime.size()-1);
	return currentTime;
}

void Logger::error(const char* text) throw()
{
	string data;
	data.append("[ERROR]: ");
	data.append(text);
	if(m_LogType == FILE_LOG)
		logIntoFile(data);
	else if(m_LogType == CONSOLE)
		logOnConsole(data);
}

void Logger::error(std::string& text) throw()
{
	error(text.data());
}

void Logger::error(std::ostringstream& stream) throw()
{
	string text = stream.str();
	error(text.data());
}

void Logger::alarm(const char* text) throw()
{
	string data;
	data.append("[ALARM]: ");
	data.append(text);
	if(m_LogType == FILE_LOG)
		logIntoFile(data);
	else if(m_LogType == CONSOLE)
		logOnConsole(data);
}

void Logger::alarm(std::string& text) throw()
{
	alarm(text.data());
}

void Logger::alarm(std::ostringstream& stream) throw()
{
	string text = stream.str();
	alarm(text.data());
}

void Logger::always(const char* text) throw()
{
	string data;
	data.append("[ALWAYS]: ");
	data.append(text);
	if(m_LogType == FILE_LOG)
		logIntoFile(data);
	else if(m_LogType == CONSOLE)
		logOnConsole(data);
}

void Logger::always(std::string& text) throw()
{
	always(text.data());
}

void Logger::always(std::ostringstream& stream) throw()
{
	string text = stream.str();
	always(text.data());
}

void Logger::buffer(const char* text) throw()
{
	if((m_LogType == FILE_LOG) && (m_LogLevel >= LOG_LEVEL_BUFFER))
	{
		lock();
		m_File << text << endl;
		unlock();
	}
	else if((m_LogType == CONSOLE) && (m_LogLevel >= LOG_LEVEL_BUFFER))
	{
		cout << text << endl;
	}
}

void Logger::buffer(std::string& text) throw()
{
	buffer(text.data());
}

void Logger::buffer(std::ostringstream& stream) throw()
{
	string text = stream.str();
	buffer(text.data());
}

void Logger::info(const char* text) throw()
{
	string data;
	data.append("[INFO]: ");
	data.append(text);

	if((m_LogType == FILE_LOG) && (m_LogLevel >= LOG_LEVEL_INFO))
	{
		logIntoFile(data);
	}
	else if((m_LogType == CONSOLE) && (m_LogLevel >= LOG_LEVEL_INFO))
	{
		logOnConsole(data);
	}
}

void Logger::info(std::string& text) throw()
{
	info(text.data());
}

void Logger::info(std::ostringstream& stream) throw()
{
	string text = stream.str();
	info(text.data());
}

void Logger::trace(const char* text) throw()
{
	string data;
	data.append("[TRACE]: ");
	data.append(text);
	if((m_LogType == FILE_LOG) && (m_LogLevel >= LOG_LEVEL_TRACE))
		logIntoFile(data);
	else if((m_LogType == CONSOLE) && (m_LogLevel >= LOG_LEVEL_TRACE))
		logOnConsole(data);

}

void Logger::trace(std::string& text) throw()
{
	trace(text.data());
}

void Logger::trace(std::ostringstream& stream) throw()
{
	string text = stream.str();
	trace(text.data());
}

void Logger::debug(const char* text) throw()
{
	string data;
	data.append("[DEBUG]: ");
	data.append(text);
	if((m_LogType == FILE_LOG) && (m_LogLevel >= LOG_LEVEL_DEBUG))
		logIntoFile(data);
	else if((m_LogType == CONSOLE) && (m_LogLevel >= LOG_LEVEL_DEBUG))
		logOnConsole(data);
}

void Logger::debug(std::string& text) throw()
{
	debug(text.data());
}

void Logger::debug(std::ostringstream& stream) throw()
{
	string text = stream.str();
	debug(text.data());
}

void Logger::updateLogLevel(LogLevel logLevel)
{
	m_LogLevel = logLevel;
}

void Logger::enaleLog()
{
	m_LogLevel = ENABLE_LOG; 
}

void Logger:: disableLog()
{
	m_LogLevel = DISABLE_LOG;
}

void Logger::updateLogType(LogType logType)
{
	m_LogType = logType;
}

void Logger::enableConsoleLogging()
{
	m_LogType = CONSOLE; 
}

void Logger::enableFileLogging()
{
	m_LogType = FILE_LOG;
}

