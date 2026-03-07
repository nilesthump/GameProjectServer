#include "Config.h"
#include "Log.h"
#include "yaml-cpp/yaml.h"

GameProjectServer::ConfigVar<int>::ptr g_int_value_config =
GameProjectServer::Config::Lookup("system.port", (int)8080, "system port");

void print_yaml(YAML::Node& node, int level)
{
	if (node.IsScalar())
	{
		NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << std::string(level * 4, ' ') <<
			node.Scalar() << " - " << node.Type() << " - " << level;
	}
	else if (node.IsNull())
	{
		NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << std::string(level * 4, ' ') <<
			"NULL - " << node.Type() << " - " << level;
	}
	else if (node.IsMap())
	{
		for (auto it : node)
		{
			NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << std::string(level * 4, ' ') <<
				it.first.Scalar() << " - " << it.first.Type() << " - " << level;
			print_yaml(it.second, level + 1);
		}
	}
	else if (node.IsSequence())
	{
		for (size_t i = 0; i < node.size(); ++i)
		{
			NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << std::string(level * 4, ' ') <<
				i << " - " << node[i].Type() << " - " << level;
			print_yaml(node[i], level + 1);
		}
	}
}

void test_yaml()
{
	try
	{
		YAML::Node root = YAML::LoadFile("H:/GameProjectServer/bin/conf/test.yml");
		print_yaml(root, 0);
		//NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << root;
	}
	catch (YAML::Exception& e)
	{
		NILESTHUMP_LOG_ERROR(NILESTHUMP_LOG_ROOT()) << "yaml parser error: " << e.what();
	}
}

void test_config()
{
	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "before; " << g_int_value_config->getValue();
	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "before: " << g_int_value_config->toString();

	YAML::Node root = YAML::LoadFile("H:/GameProjectServer/bin/conf/test.yml");
	GameProjectServer::Config::LoadFromYaml(root);

	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "after: " << g_int_value_config->toString();
}

int main(int argc, char** argv)
{
	test_config();
	test_yaml();
	return 0;
}