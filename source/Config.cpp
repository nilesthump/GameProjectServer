#include "Config.h"

namespace GameProjectServer
{
	ConfigVarBase::ptr Config::LookupBase(const std::string& name)
	{
		auto it = s_datas.find(name);
		return it == s_datas.end() ? nullptr : it->second;
	}

	static inline void ListAllMember(
		const std::string& prefix,
		const YAML::Node& node,
		std::list<std::pair<std::string, const YAML::Node>>& output)
	{
		boost::regex pattern("^[a-zA-Z0-9\\._]*$");
		if (!boost::regex_match(prefix, pattern))
		{
			NILESTHUMP_LOG_ERROR(NILESTHUMP_LOG_ROOT()) << "Config invalid prefix: " << prefix << " : " << node;
			return;
		}
		output.push_back(std::make_pair(prefix, node));
		if (node.IsMap())
		{
			for (auto it = node.begin(); it != node.end(); ++it)
			{
				ListAllMember(prefix.empty() ? it->first.Scalar() :
					prefix + "." + it->first.Scalar(), it->second, output);
			}
		}
	}

	void Config::LoadFromYaml(const YAML::Node& root)
	{
		std::list<std::pair<std::string, const YAML::Node>> all_nodes;
		ListAllMember("", root, all_nodes);
		for (auto& i : all_nodes)
		{
			const std::string& key = i.first;
			if (key.empty())
			{
				continue;
			}
			ConfigVarBase::ptr var = LookupBase(key);

			if (var)
			{
				if (i.second.IsScalar())
				{
					var->fromString(i.second.Scalar());
				}
				else
				{
					std::stringstream ss;
					ss << i.second;
					var->fromString(ss.str());
					//NILESTHUMP_LOG_ERROR(NILESTHUMP_LOG_ROOT()) << "Config invalid value: " << i.second << " for key: " << key;
				}
			}
		}
	}
}