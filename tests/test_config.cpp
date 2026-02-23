#include "Config.h"
#include "Log.h"

GameProjectServer::ConfigVar<int>::ptr g_int_value_config =
	GameProjectServer::Config::Lookup("system.port", (int)8080, "system port");

int main(int argc, char** argv)
{
	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "before: " << g_int_value_config->toString();
	return 0;
}