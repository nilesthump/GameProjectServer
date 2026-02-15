#define NILESTHUMP_RETURN_ROOT
#include <iostream>
#include "Util.h"
#include "Log.h"

int main() {
	GameProjectServer::Logger::ptr logger = std::make_shared<GameProjectServer::Logger>("test_logger");
	logger->addAppender(std::make_shared<GameProjectServer::StdoutLogAppender>());
	/*
	GameProjectServer::LogEvent::ptr event = std::make_shared<GameProjectServer::LogEvent>(
		__FILE__, __LINE__, 12345, GameProjectServer::GetThreadId(), GameProjectServer::GetFiberId(), time(0)
	);*/
	//event->getSS() << "This is a test log message.";
	//logger->log(GameProjectServer::LogLevel::DEBUG, event);
	GameProjectServer::FileLogAppender::ptr file_appender = std::make_shared<GameProjectServer::FileLogAppender>("test_log.txt");
	file_appender->setLevel(GameProjectServer::LogLevel::ERROR);
	logger->addAppender(file_appender);
	NILESTHUMP_LOG_INFO(logger) << "test macro log message";

	NILESTHUMP_LOG_FMT_ERROR(logger, "Formatted log message: %d, %s", 42, "hello");

	auto l = GameProjectServer::LoggerMgr::GetInstance()->getLogger("xx");
	NILESTHUMP_LOG_INFO(l) << "test macro log message xx";

	std::cout << "Log event logged successfully." << std::endl;
	return 0;
}