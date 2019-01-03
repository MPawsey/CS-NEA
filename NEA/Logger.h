#pragma once

#include <string>
#include <chrono>
#include <iostream>
#include <Windows.h>



#define LOGTYPES \
o(None, "\0", 15) \
o(Info, "Info\0", 11) \
o(Warning, "Warning\0", 14) \
o(Error, "Error\0", 12)

class Logger
{
public:
	#define o(id, name, code) lt##id,
	enum LogType
	{
		LOGTYPES
	};
	#undef o
		
private:
	const std::string name;

	struct oLogType
	{
		LogType id;
		std::string name;
		int code;
	};

	#define o(id, name, code) {lt##id, name, code},
	static inline const oLogType logTypes[] =
	{
		LOGTYPES
	};
	#undef o

public:

	Logger(std::string name)
		: name(name)
	{

	}

	void Log(std::string text, LogType type = ltNone)
	{
		if (type != ltNone)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			char s[80];
			tm* timeinfo = new tm{};
			localtime_s(timeinfo, &tt);
			strftime(s, 80, "%F %T", timeinfo);

			std::cout << s << " [" << name << "] ";
			SetConsoleTextAttribute(hConsole, logTypes[type].code);
			std::cout << logTypes[type].name << " ";
			SetConsoleTextAttribute(hConsole, 15);
		}
		std::cout << text << std::endl;
	}

	void Info(std::string text)
	{
		Log(text, ltInfo);
	}

	void Warning(std::string text)
	{
		Log(text, ltWarning);
	}

	void Error(std::string text)
	{
		Log(text, ltError);
	}

};


#undef LOGTYPES