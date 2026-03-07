#include "Config.h"
#include "Log.h"
#include "yaml-cpp/yaml.h"

GameProjectServer::ConfigVar<int>::ptr g_int_value_config =
GameProjectServer::Config::Lookup("system.port", (int)8080, "system port");

GameProjectServer::ConfigVar<float>::ptr g_float_value_config =
GameProjectServer::Config::Lookup("system.value", (float)10.2f, "system value");

GameProjectServer::ConfigVar<std::vector<int>>::ptr g_int_vector_config =
GameProjectServer::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vector");

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
	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "before: " << g_float_value_config->toString();
	auto v = g_int_vector_config->getValue();
	for (auto& i : v)
	{
		NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "before int_vec: " << i;
	}

	YAML::Node root = YAML::LoadFile("H:/GameProjectServer/bin/conf/test.yml");
	GameProjectServer::Config::LoadFromYaml(root);

	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "after: " << g_float_value_config->toString();

	v = g_int_vector_config->getValue();
	for (auto& i : v)
	{
		NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "after int_vec: " << i;
	}
}

int main(int argc, char** argv)
{
	test_config();
	//test_yaml();
	return 0;
}