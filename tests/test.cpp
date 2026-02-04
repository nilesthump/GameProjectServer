#include <iostream>
#include "Log.h"

int main() {
	GameProjectServer::Logger::ptr logger = std::make_shared<GameProjectServer::Logger>("test_logger");
	logger->addAppender(std::make_shared<GameProjectServer::StdoutLogAppender>());

	GameProjectServer::LogEvent::ptr event = std::make_shared<GameProjectServer::LogEvent>(
		__FILE__, __LINE__, 0, 0, 0, std::u16streampos()
	);

	logger->log(GameProjectServer::LogLevel::DEBUG, event);

	std::cout << "Log event logged successfully." << std::endl;
	return 0;
}