#include "Config.h"
#include "Log.h"
#include "yaml-cpp/yaml.h"

GameProjectServer::ConfigVar<int>::ptr g_int_value_config =
GameProjectServer::Config::Lookup("system.port", (int)8080, "system port");

GameProjectServer::ConfigVar<float>::ptr g_float_value_config =
GameProjectServer::Config::Lookup("system.value", (float)10.2f, "system value");

GameProjectServer::ConfigVar<std::vector<int>>::ptr g_int_vector_config =
GameProjectServer::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vector");

GameProjectServer::ConfigVar<std::list<int>>::ptr g_int_list_config =
GameProjectServer::Config::Lookup("system.int_list", std::list<int>{1, 2}, "system int list");

GameProjectServer::ConfigVar<std::set<int>>::ptr g_int_set_config =
GameProjectServer::Config::Lookup("system.int_set", std::set<int>{1, 2}, "system int set");

GameProjectServer::ConfigVar<std::unordered_set<int>>::ptr g_int_uset_config =
GameProjectServer::Config::Lookup("system.int_uset", std::unordered_set<int>{1, 2}, "system int unordered set");

GameProjectServer::ConfigVar<std::map<std::string, int>>::ptr g_str_int_map_config =
GameProjectServer::Config::Lookup("system.str_int_map", std::map<std::string, int>{{"k", 1}, { "k2", 2 }}, "system string int map");

GameProjectServer::ConfigVar<std::unordered_map<std::string, int>>::ptr g_str_int_ump_config =
GameProjectServer::Config::Lookup("system.str_int_ump", std::unordered_map<std::string, int>{{"k", 1}, { "k2", 2 }}, "system string int unordered map");

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
#define XX(g_var, name, prefix) \
	{\
	auto& v = g_var->getValue(); \
	for (auto& i : v)\
	{\
		NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << #prefix " " #name ": " << i;\
	}\
		NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString();\
	}

#define XX_MAP(g_var, name, prefix) \
	{\
	auto& m = g_var->getValue(); \
	for (auto& i : m)\
	{\
		NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << #prefix " " #name ": {" << i.first << " - " << i.second << "}";\
	}\
		NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString();\
	}

	XX(g_int_vector_config, int_vec, before);
	XX(g_int_list_config, int_list, before);
	XX(g_int_set_config, int_set, before);
	XX(g_int_uset_config, int_uset, before);
	XX_MAP(g_str_int_map_config, str_int_map, before);
	XX_MAP(g_str_int_ump_config, str_int_ump, before);

	YAML::Node root = YAML::LoadFile("H:/GameProjectServer/bin/conf/test.yml");
	GameProjectServer::Config::LoadFromYaml(root);

	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
	NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "after: " << g_float_value_config->toString();

	XX(g_int_vector_config, int_vec, after);
	XX(g_int_list_config, int_list, after);
	XX(g_int_set_config, int_set, after);
	XX(g_int_uset_config, int_uset, after);
	XX_MAP(g_str_int_map_config, str_int_map, after);
	XX_MAP(g_str_int_ump_config, str_int_ump, after);
#undef XX_MAP
#undef XX
}

int main(int argc, char** argv)
{
	test_config();
	//test_yaml();
	return 0;
}