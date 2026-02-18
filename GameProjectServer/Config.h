#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace GameProjectServer
{
	class ConfigVarBase
	{
	public:
		typedef std::shared_ptr<ConfigVarBase> ptr;
		ConfigVarBase(const std::string& name, const std::string& description = "")
			: m_name(name)
			, m_description(description)
		{
		}
		virtual ~ConfigVarBase() {}

		std::string getName() const { return m_name; }
		std::string getDescription() const { return m_description; }

		virtual std::string toString() = 0;			//将配置项转换为字符串
		virtual bool fromString(const std::string& val) = 0;		//解析字符串，更新配置项的值
	protected:
		std::string m_name;
		std::string m_description;
	};

	template<class T>
	class ConfigVar : public ConfigVarBase
	{
	public:
		typedef std::shared_ptr<ConfigVar> ptr;
		ConfigVar(const std::string& name, const T& default_value, const std::string& description = "")
			: ConfigVarBase(name, description)
			, m_val(default_value)
		{
		}
		virtual std::string toString() override
		{
			try
			{
				return boost::lexical_cast<std::string>(m_val);
			}
			catch (boost::bad_lexical_cast& e)
			{
				return "";
			}
		}
		virtual bool fromString(const std::string& val) override
		{
			try
			{
				m_val = boost::lexical_cast<T>(val);
				return true;
			}
			catch (boost::bad_lexical_cast& e)
			{
				return false;
			}
		}
		const T getValue() const { return m_val; }
	}

}