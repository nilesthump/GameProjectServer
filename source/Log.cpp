// GameProjectServer.cpp: 定义应用程序的入口点。
//

#include "Log.h"
#include "Config.h"
#include <tuple>
#include <iostream>
#include <cctype>
#include <functional>
#include <ctime>
#include <cstdio>

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
			os << event->getLogger()->getName();
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

	LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
		const char* file, uint32_t line, uint32_t elapse,
		uint32_t thread_id, uint32_t fiber_id, std::u16streampos time)
		: m_logger(logger), m_level(level), m_file(file), m_line(line),
		m_elapse(elapse), m_threadId(thread_id), m_fiberId(fiber_id), m_time(time)
	{
	}

	void LogEvent::format(const char* fmt, ...)
	{
		va_list al;
		va_start(al, fmt);
		format(fmt, al);
		va_end(al);
	}

	void LogEvent::format(const char* fmt, va_list al)
	{
		char* buf = nullptr;

		auto vasprintf = [&fmt, &al](char** str)->int 
			{
			va_list args;
			va_copy(args, al);
			int len = vsnprintf(nullptr, 0, fmt, args);
			va_end(args);
			if (len < 0)
			{
				return -1;
			}
			*str = new char[len + 1];
			if (*str == nullptr)
			{
				return -1;
			}
			return vsnprintf(*str, len + 1, fmt, al);
			};

		if (int len = vasprintf(&buf); len != -1)
		{
			m_ss << std::string(buf, len);
			free(buf);
		}
	}

	Logger::Logger(const std::string& name)
		: m_name(name), m_level(LogLevel::DEBUG)
	{
		m_formatter.reset(new LogFormatter("%d{%H:%M:%S %Y-%m-%d}%T%t%T%F%T[%p]%T[%c]%T<%f:%l>%T%m%n")); //默认格式
	
		if (name == "root")
		{
			m_appenders.push_back(std::make_shared<StdoutLogAppender>());
		}
	}

	void Logger::setFormatter(LogFormatter::ptr formatter)
	{
		m_formatter = formatter;
	}

	void Logger::setFormatter(const std::string& pattern)
	{
		LogFormatter::ptr new_formatter(new LogFormatter(pattern));
		if (new_formatter->isError())
		{
			std::cout << "Logger setFormatter name=" << m_name
				<< " value=" << pattern << " invalid formatter" << std::endl;
			return;
		}
		m_formatter = new_formatter;
	}

	LogFormatter::ptr Logger::getFormatter() const
	{
		return m_formatter;
	}

	LogLevel::Level LogLevel::FromString(const std::string& str) 
	{
#define XX(level) \
		if (str == #level) {\
			return LogLevel::level;\
		}
		XX(DEBUG)
		XX(INFO)
		XX(WARN)
		XX(ERROR)
		XX(FATAL)
#undef XX
		return LogLevel::UNKNOW;
	}

	const char* LogLevel::ToString(Level level) {
		switch (level) 
		{
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

	LogEventWrap::LogEventWrap(LogEvent::ptr e)
		: m_event(e)
	{
	}

	LogEventWrap::~LogEventWrap()
	{
		m_event->getLogger()->log(m_event->getLevel(), m_event);
	}

	void Logger::log(LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			auto self = shared_from_this();
			if (!m_appenders.empty())
			{
				for (auto& appender : m_appenders)
				{
					appender->log(self, level, event);
				}
			}
			else if (m_root)
			{
				m_root->log(level, event);
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

	void Logger::clearAppenders()
	{
		m_appenders.clear();
	}

	FileLogAppender::FileLogAppender(const std::string& filename)
		: m_filename(filename)
	{
		reopen();
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
				m_error = true;
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
				if (auto it = s_format_items.find(std::get<0>(i)); it == s_format_items.end())
				{
					m_items.push_back(FormatItem::ptr(std::make_shared<StringFormatItem>("<<error_format %" + std::get<0>(i) + ">>")));
					m_error = true;
				}
				else
				{
					m_items.push_back(it->second(std::get<1>(i)));
				}
			}
		}

	}

	LoggerManager::LoggerManager()
	{
		m_root.reset(new Logger);
		m_root->addAppender(std::make_shared<StdoutLogAppender>());
		
		init();
	}

	Logger::ptr LoggerManager::getLogger(const std::string& name)
	{
		auto it = m_loggers.find(name);
		if (it != m_loggers.end())
		{
			return it->second;
		}
		Logger::ptr logger(new Logger(name));
		logger->m_root = m_root;
		m_loggers[name] = logger;
		return logger;
	}

	struct LogAppenderDefine
	{
		int type = 0;                            //1 File 2 Stdout
		LogLevel::Level level = LogLevel::UNKNOW;                           //日志级别
		std::string formatter;                  //日志格式
		std::string file;                       //当type = 1时，file为必须项

		bool operator==(const LogAppenderDefine& oth) const
		{
			return type == oth.type
				&& level == oth.level
				&& formatter == oth.formatter
				&& file == oth.file;
		}
	};

	struct LogDefine
	{
		std::string name;                            //日志器名称
		LogLevel::Level level = LogLevel::UNKNOW;                         //日志器级别
		std::string formatter;                      //日志格式器
		std::vector<LogAppenderDefine> appenders; //日志输出地集合

		bool operator==(const LogDefine& oth) const
		{
			return name == oth.name
				&& level == oth.level
				&& formatter == oth.formatter
				&& appenders == oth.appenders;
		}

		bool operator<(const LogDefine& oth) const
		{
			return name < oth.name;
		}
	};

	template<>
	class LexicalCast<std::string, LogDefine>
	{
	public:
		LogDefine operator()(const std::string& v)
		{
			YAML::Node node = YAML::Load(v);
			if (!node["name"].IsDefined())
			{
				throw std::invalid_argument("log config must have name");
				//NILESTHUMP_LOG_ERROR(NILESTHUMP_LOG_ROOT()) << "log config must have name";
			}
			LogDefine ld;
			ld.name = node["name"].as<std::string>();
			ld.level = LogLevel::FromString(node["level"].IsDefined() ?
				node["level"].as<std::string>() : "");
			ld.formatter = node["formatter"].IsDefined() ? node["formatter"].as<std::string>() : "";
			if (node["appenders"].IsDefined())
			{
				for (size_t i = 0; i < node["appenders"].size(); ++i)
				{
					auto a = node["appenders"][i];
					if (!a["type"].IsDefined())
					{
						std::cout << "log appender config error: type is required" << std::endl;
						continue;
					}
					std::string type = a["type"].as<std::string>();
					LogAppenderDefine lad;
					if (type == "FileLogAppender")
					{
						lad.type = 1;
						if (!a["file"].IsDefined())
						{
							std::cout << "log appender config error: file is required for FileLogAppender" << std::endl;
							continue;
						}
						lad.file = a["file"].as<std::string>();
						if (a["formatter"].IsDefined())
						{
							lad.formatter = a["formatter"].as<std::string>();
						}
					}
					else if (type == "StdoutLogAppender")
					{
						lad.type = 2;
					}
					else
					{
						std::cout << "log appender config error: type is invalid" << std::endl;
						continue;
					}

					ld.appenders.push_back(lad);
				}
			}
			return ld;
		}
	};

	ConfigVar<std::set<LogDefine>>::ptr g_log_defines =
		Config::Lookup("logs", std::set<LogDefine>(), "logs config");

	struct LogIniter 
	{
		LogIniter()
		{
			g_log_defines->addListener(0xF1E231, [](const std::set<LogDefine>& old_value,
				const std::set<LogDefine>& new_value) {
					NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "on_logger_conf_changed";
					//新增
					for (auto& i : new_value)
					{
						auto it = old_value.find(i);
						Logger::ptr logger;
						if (it == old_value.end())
						{
							//新增logger
							logger.reset(new Logger(i.name));
						}
						else
						{
							if (!(i == *it))
							{
								//修改logger
								//先删除原有的appender
								logger = NILESTHUMP_LOG_GET_LOGGER(i.name);
							}
						}
						logger->setLevel(i.level);
						if (!i.formatter.empty())
						{
							logger->setFormatter(i.formatter);
						}

						logger->clearAppenders();
						for (auto& a : i.appenders)
						{
							LogAppender::ptr appender;
							if (a.type == 1)
							{
								appender.reset(new FileLogAppender(a.file));
							}
							else if (a.type == 2)
							{
								appender.reset(new StdoutLogAppender);
							}
							appender->setLevel(a.level);
							logger->addAppender(appender);
						}
					}
					//修改
					//删除
					for (auto& i : old_value)
					{
						auto it = new_value.find(i);
						if (it == new_value.end())
						{
							//删除logger
							auto logger = NILESTHUMP_LOG_GET_LOGGER(i.name);
							logger->setLevel((LogLevel::Level)100);
							logger->clearAppenders();
						}
						else
						{
							if (!(i == *it))
							{
								//修改logger
								//先删除原有的appender
							}
						}
					}
				});
		}
	};

	static LogIniter __log_init;

	void LoggerManager::init()
	{

	}

} // namespace GameProjectServer
