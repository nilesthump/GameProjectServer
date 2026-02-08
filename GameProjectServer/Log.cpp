// GameProjectServer.cpp: 定义应用程序的入口点。
//

#include "Log.h"
#include <tuple>
#include <iostream>
#include <cctype>
#include <functional>
#include <map>
#include <ctime>

namespace GameProjectServer
{

	class MessageFormatItem : public LogFormatter::FormatItem {
	public:
		MessageFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << event->getMessage();
		}
	};

	class LevelFormatItem : public LogFormatter::FormatItem {
	public:
		LevelFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << LogLevel::ToString(level);
		}
	};

	class ElapseFormatItem : public LogFormatter::FormatItem {
	public:
		ElapseFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << event->getElapse();
		}
	};

	class NameFormatItem : public LogFormatter::FormatItem {
	public:
		NameFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << logger->getName();
		}
	};

	class ThreadIdFormatItem : public LogFormatter::FormatItem {
	public:
		ThreadIdFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << event->getThreadId();
		}
	};

	class FiberIdFormatItem : public LogFormatter::FormatItem {
	public:
		FiberIdFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << event->getFiberId();
		}
	};

	class DateTimeFormatItem : public LogFormatter::FormatItem {
	public:
		DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
			: m_format(format)
		{
			if (m_format.empty())
			{
				m_format = "%Y-%m-%d %H:%M:%S";
			}
		}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			tm tm_time;
			time_t t = event->getTime();
			localtime_s(&tm_time, &t);
			char buf[64];
			strftime(buf, sizeof(buf), m_format.c_str(), &tm_time);
			os << buf;
		}
	private:
		std::string m_format;
	};

	class FilenameFormatItem : public LogFormatter::FormatItem {
	public:
		FilenameFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << event->getFile();
		}
	};

	class NewLineFormatItem : public LogFormatter::FormatItem {
	public:
		NewLineFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << std::endl;
		}
	};

	class LineFormatItem : public LogFormatter::FormatItem {
	public:
		LineFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << event->getLine();
		}
	};

	class StringFormatItem : public LogFormatter::FormatItem {
	public:
		StringFormatItem(const std::string& str)
			: m_string(str)
		{
		}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << m_string;
		}
	private:
		std::string m_string;
	};

	class TabFormatItem : public LogFormatter::FormatItem {
		public:
		TabFormatItem(const std::string& str = "") {}
		void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override {
			os << "\t";
		}
	};

	LogEvent::LogEvent(const char* file, uint32_t line, uint32_t elapse,
		uint32_t thread_id, uint32_t fiber_id, std::u16streampos time)
		: m_file(file), m_line(line), m_elapse(elapse),
		m_threadId(thread_id), m_fiberId(fiber_id), m_time(time)
	{
	}

	Logger::Logger(const std::string& name)
		: m_name(name), m_level(LogLevel::DEBUG)
	{
		m_formatter.reset(new LogFormatter("%d{%H:%M:%S %Y-%m-%d}%T%t%T%F%T[%p]%T[%c]%T<%f:%l>%T%m%n"));            //默认格式
	}

	const char* LogLevel::ToString(Level level) {
		switch (level) {
#define XX(name)\
			case LogLevel::name:\
				return #name;
			XX(DEBUG)
			XX(INFO)
			XX(WARN)
			XX(ERROR)
			XX(FATAL)
#undef XX
			default:
				return "UNKNOW";
		}
	}

	void Logger::log(LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			auto self = shared_from_this();
			for (auto& appender : m_appenders)
			{
				appender->log(self, level, event);
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
		if (!appender->getFormatter())
		{
			appender->setFormatter(m_formatter);
		}
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

	void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			m_filestream << m_formatter->format(logger, level, event);
		}
	}

	void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			std::cout << m_formatter->format(logger, level, event);
		}
	}

	LogFormatter::LogFormatter(const std::string& pattern)
		: m_pattern(pattern)
	{
		init();
	}

	std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		std::stringstream ss;
		for (auto& item : m_items)
		{
			item->format(ss, logger, level, event);
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
					if (!isalpha(m_pattern[n]) && m_pattern[n] != '{')
					{
						break;
					}
					else if (m_pattern[n] == '{')
					{
						str_type = m_pattern.substr(i + 1, n - i - 1);
						fmt_status = 1;          //进入格式解析状态
						fmt_begin = n + 1;
					}
				}
				else if (fmt_status == 1)
				{
					if (m_pattern[n] == '}')
					{
						fmt = m_pattern.substr(fmt_begin, n - fmt_begin);
						fmt_status = 2;		  //格式解析完成
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
					vec.push_back(std::make_tuple(str_type, fmt, 1));           //fmt为空
				}
				i = n - 1;
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
		if (!nstr.empty())
		{
			vec.push_back(std::make_tuple(nstr, "", 0));
			nstr.clear();
		}

		/******************************
			%m -- 消息体
			%p -- 日志级别
			%r -- 累计毫秒数
			%c -- 日志名称
			%t -- 线程id
			%n -- 换行
			%d -- 时间
			%f -- 文件名
			%l -- 行号
		******************************/
		static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str, C) \
			{ #str, [](const std::string& fmt) { return FormatItem::ptr(std::make_shared<C>(fmt)); } }
			XX(m, MessageFormatItem),
			XX(p, LevelFormatItem),
			XX(r, ElapseFormatItem),
			XX(c, NameFormatItem),
			XX(t, ThreadIdFormatItem),
			XX(n, NewLineFormatItem),
			XX(d, DateTimeFormatItem),
			XX(f, FilenameFormatItem),
			XX(l, LineFormatItem),
			XX(T, TabFormatItem),
			XX(F, FiberIdFormatItem)
#undef XX
		};

		for (auto& i : vec)
		{
			if (std::get<2>(i) == 0)
			{
				m_items.push_back(FormatItem::ptr(std::make_shared<StringFormatItem>(std::get<0>(i))));
			}
			else
			{
				auto it = s_format_items.find(std::get<0>(i));
				if (it == s_format_items.end())
				{
					m_items.push_back(FormatItem::ptr(std::make_shared<StringFormatItem>("<<error_format %" + std::get<0>(i) + ">>")));
				}
				else
				{
					m_items.push_back(it->second(std::get<1>(i)));
				}
			}
		}

	}



} // namespace GameProjectServer
