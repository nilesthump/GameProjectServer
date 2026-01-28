// GameProjectServer.cpp: 定义应用程序的入口点。
//

#include "Log.h"
#include <tuple>
#include <sstream>
#include <iostream>
#include <cctype>

namespace GameProjectServer
{
	Logger::Logger(const std::string& name)
		: m_name(name)
	{
	}

	void Logger::log(LogLevel level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			for(auto& appender : m_appenders)
			{
				appender->log(level, event);
			}
		}
	}

	void Logger::debug(LogEvent::ptr event)
	{
		log(LogLevel::DEBUG, event);
	}

	void Logger::info(LogEvent::ptr event)
	{
		log(LogLevel::INFO, event);
	}

	void Logger::warn(LogEvent::ptr event)
	{
		log(LogLevel::WARN, event);
	}

	void Logger::error(LogEvent::ptr event)
	{
		log(LogLevel::ERROR, event);
	}

	void Logger::fatal(LogEvent::ptr event)
	{
		log(LogLevel::FATAL, event);
	}

	void Logger::addAppender(LogAppender::ptr appender)
	{
		m_appenders.push_back(appender);
	}
	void Logger::delAppender(LogAppender::ptr appender)
	{
		m_appenders.remove(appender);
	}

	FileLogAppender::FileLogAppender(const std::string& filename)
		: m_filename(filename)
	{
	}

	bool FileLogAppender::reopen()
	{
		if (m_filestream)
		{
			m_filestream.close();
		}
		m_filestream.open(m_filename);
		return !!m_filestream;
	}

	void FileLogAppender::log(LogLevel level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			m_filestream << m_formatter->format(event);
		}
	}

	void StdoutLogAppender::log(LogLevel level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			std::cout << m_formatter->format(event);
		}
	}

	LogFormatter::LogFormatter(const std::string& pattern)
		: m_pattern(pattern)
	{
		init();
	}

	std::string LogFormatter::format(LogEvent::ptr event)
	{
		std::stringstream ss;
		for (auto& item : m_items)
		{
			item->format(ss, event);
		}
		return ss.str();
	}

	/**********************************
		important!!!
		ps:%xxx		%xxx{xxx}	%%转义
	**********************************/
	void LogFormatter::init()
	{
		/*  string	format	type  */
		std::vector<std::tuple<std::string, std::string, int>> vec;
		std::string str;
		for (size_t i = 0; i < m_pattern.size(); ++i)
		{
			if (m_pattern[i] != '%')
			{
				str.append(1, m_pattern[i]);
				continue;
			}
			size_t n = i + 1;
			int fmt_status = 0;

			std::string fmt;
			std::string str_type;
			while (n < m_pattern.size())
			{
				if (m_pattern[n] == '%')
				{
					str.append(1, '%');
					i = n;
					break;
				}
				else if (isspace(m_pattern[n]))
				{
					i = n;
					break;
				}
				if (fmt_status == 0)
				{
					if (m_pattern[n] == '{')
					{
					}
				}
				else if (fmt_status == 1)
				{
					if (m_pattern[n] == '}')
					{
					}
				}
				n++;
			}
		}
	}

} // namespace GameProjectServer
