#include <iostream>
#include "Util.h"
#include "Log.h"


int main() {
	GameProjectServer::Logger::ptr logger = std::make_shared<GameProjectServer::Logger>("test_logger");
	logger->addAppender(std::make_shared<GameProjectServer::StdoutLogAppender>());

	GameProjectServer::LogEvent::ptr event = std::make_shared<GameProjectServer::LogEvent>(
		__FILE__, __LINE__, 12345, GameProjectServer::GetThreadId(), 90, time(0)
	);
	event->getSS() << "This is a test log message.";
	logger->log(GameProjectServer::LogLevel::DEBUG, event);

	std::cout << "Log event logged successfully." << std::endl;
	return 0;
}