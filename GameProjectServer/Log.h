// GameProjectServer.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。
#pragma once

#include <string>
#include <cstdint>
#include <ios>
#include <memory>

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
		u16streampos m_time;            //时间戳
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
		//为appender提供event信息，返回格式化后的字符串
		std::string format(LogEvent::ptr event);
	private:

	};


	//日志输出地
	class LogAppender {
	public:
		typedef std::shared_ptr<LogAppender> ptr;

		//虚析构函数，确保派生类正确析构
		virtual ~LogAppender() = 0;
		void log(LogLevel level, LogEvent::ptr event);
	private:
		LogLevel m_level;               //日志输出级别
	};

	//日志器
	class Logger {
	public:
		typedef std::shared_ptr<Logger> ptr;

		Logger(const std::string& name = "root");

		void log(LogLevel level, LogEvent::ptr event);
	private:
		std::string m_name;                        //日志器名称
		LogLevel m_level;                         //日志器级别
		LogAppender::ptr;
	};

	//输出到控制台的日志输出地
	class StdoutLogAppender : public LogAppender {


	};

	//输出到文件的日志输出地
	class FileLogAppender : public LogAppender {


	};
}

// TODO: 在此处引用程序需要的其他标头。
