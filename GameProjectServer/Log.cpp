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
		type: 0:字符串	1:格式化字符串
	**********************************/
	void LogFormatter::init()
	{
		/************************************************  
			string			format		type  
			读取的字符串	解析格式	类型
		************************************************/
		std::vector<std::tuple<std::string, std::string, int>> vec;
		std::string nstr;
		for (size_t i = 0; i < m_pattern.size(); ++i)
		{
			if (m_pattern[i] != '%')
			{
				nstr.append(1, m_pattern[i]);
				continue;
			}
			if (i + 1 < m_pattern.size())
			{
				if (m_pattern[i + 1] == '%')
				{
					nstr.append(1, '%');
					++i;
					continue;
				}
			}

			size_t n = i + 1;
			int fmt_status = 0;
			size_t fmt_begin = 0;

			std::string fmt;
			std::string str_type;
			while (n < m_pattern.size())
			{
				if (fmt_status == 0)
				{
					if (isspace(m_pattern[n]))
					{
						i = n;
						break;
					}
					else if (m_pattern[n] == '{')
					{
						str_type = m_pattern.substr(i + 1, n - i - 1);
						fmt_status = 1;
						fmt_begin = n + 1;
					}
				}
				else if (fmt_status == 1)
				{
					if (m_pattern[n] == '}')
					{
						fmt = m_pattern.substr(fmt_begin, n - fmt_begin);
						i = n;
						break;
					}
				}
				n++;
			}
			if (fmt_status == 0)
			{
				if (!nstr.empty())
				{
					vec.push_back(std::make_tuple(nstr, "", 0));
					nstr.clear();
				}
				str_type = m_pattern.substr(i + 1, n - i - 1);
				if (!str_type.empty())
				{
					vec.push_back(std::make_tuple(str_type, fmt, 1));
				}
			}
			else if (fmt_status == 1)
			{
				std::cout << "pattern parse error:" << m_pattern << '-' << m_pattern.substr(i) << std::endl;
				vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
			}
			else if (fmt_status == 2)
			{
				if (!nstr.empty())
				{
					vec.push_back(std::make_tuple(nstr, "", 0));
					nstr.clear();
				}
				if (!str_type.empty())
				{
					vec.push_back(std::make_tuple(str_type, fmt, 1));
				}
			}

		}
	}

} // namespace GameProjectServer
