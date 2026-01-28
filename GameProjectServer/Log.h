// GameProjectServer.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。
#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <list>
#include <ios>
#include <fstream>
#include <vector>
#include <ostream>

namespace GameProjectServer
{
	
	//日志事件
	class LogEvent {
	public:
		typedef std::shared_ptr<LogEvent> ptr;
		LogEvent();

	private:
		const char* m_file = nullptr;      //日志事件发生的文件
		uint32_t m_line = 0;           //日志事件发生的行号
		uint32_t m_elapse = 0;         //程序启动到现在的毫秒数
		uint32_t m_threadId = 0;      //线程ID
		uint32_t m_fiberId = 0;       //协程ID
		std::u16streampos m_time;            //时间戳
		std::string m_message;      //日志消息
	};

	//日志级别
	enum class LogLevel {
		UNKNOW = 0,
		DEBUG = 1,
		INFO = 2,
		WARN = 3,
		ERROR = 4,
		FATAL = 5
	};

	//日志格式化器
	class LogFormatter {
	public:
		typedef std::shared_ptr<LogFormatter> ptr;
		LogFormatter(const std::string& pattern);
		/***************************************************
			为appender提供event信息，返回格式化后的字符串
			%t:时间		%threadid:线程号	%m:消息   
			%p:日志级别		%n:换行符		%f:文件名
		***************************************************/
		std::string format(LogEvent::ptr event);
	private:
		class FormatItem {
		public:
			typedef std::shared_ptr<FormatItem> ptr;
			virtual ~FormatItem() {}
			virtual void format(std::ostream& os, LogEvent::ptr event) = 0;
		};

		void init();
	private:
		std::string m_pattern;                      //日志格式模板
		std::vector<FormatItem::ptr> m_items; //日志格式化规则集合

	};


	//日志输出地
	class LogAppender {
	public:
		typedef std::shared_ptr<LogAppender> ptr;

		//虚析构函数，确保派生类正确析构
		virtual ~LogAppender() {}
		virtual void log(LogLevel level, LogEvent::ptr event) = 0;

		void setFormatter(LogFormatter::ptr formatter) { m_formatter = formatter; }
		LogFormatter::ptr getFormatter() const { return m_formatter; }
	protected:
		LogLevel m_level;               //日志输出级别
		LogFormatter::ptr m_formatter; //日志格式化器
	};

	//日志器
	class Logger {
	public:
		typedef std::shared_ptr<Logger> ptr;

		Logger(const std::string& name = "root");

		void log(LogLevel level, LogEvent::ptr event);

		void debug(LogEvent::ptr event);
		void info(LogEvent::ptr event);
		void warn(LogEvent::ptr event);
		void error(LogEvent::ptr event);
		void fatal(LogEvent::ptr event);

		void addAppender(LogAppender::ptr appender);
		void delAppender(LogAppender::ptr appender);
		LogLevel getLevel() const { return m_level; }
		void setLevel(LogLevel level) { m_level = level; }
	private:
		std::string m_name;                        //日志器名称
		LogLevel m_level;                         //日志器级别
		std::list<LogAppender::ptr> m_appenders; //日志输出地集合
	};

	//输出到控制台的日志输出地
	class StdoutLogAppender : public LogAppender {
	public:
		typedef std::shared_ptr<StdoutLogAppender> ptr;
		virtual void log(LogLevel level, LogEvent::ptr event) override;

	private:

	};

	//输出到文件的日志输出地
	class FileLogAppender : public LogAppender {
	public:
		typedef std::shared_ptr<FileLogAppender> ptr;
		FileLogAppender(const std::string& filename);
		virtual void log(LogLevel level, LogEvent::ptr event) override;

		//重新打开文件，文件打开失败返回false
		bool reopen();
	private:
		std::string m_filename;    //日志文件名
		std::ofstream m_filestream; //文件输出流
	};
}

// TODO: 在此处引用程序需要的其他标头。
