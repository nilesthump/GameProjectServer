#pragma once

#include <memory>
#include <string>
#include <exception>
#include <map>
#include <sstream>
#include <regex>
#include <boost/lexical_cast.hpp>
#include "Log.h"

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
		std::string toString() override
		{
			try
			{
				return boost::lexical_cast<std::string>(m_val);
			}
			catch (std::exception& e)
			{
				NILESTHUMP_LOG_ERROR(NILESTHUMP_LOG_ROOT()) << "ConfigVar::toString exception "
					<< e.what() << " convert: " << typeid(m_val).name() << " to string";
			}
			return "";
		}
		virtual bool fromString(const std::string& val) override
		{
			try
			{
				m_val = boost::lexical_cast<T>(val);
				return true;
			}
			catch (std::exception& e)
			{
				NILESTHUMP_LOG_ERROR(NILESTHUMP_LOG_ROOT()) << "ConfigVar::toString exception "
					<< e.what() << " convert: string to " << typeid(m_val).name();
			}
			return false;
		}
		const T getValue() const { return m_val; }
		void setValue(const T& v) { m_val = v; }
	private:
		T m_val;
	};

	class Config
	{
	public:
		typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;
		
		/******************************************
			用于查找或创建一个配置项，
			如果配置项存在，
			返回已存在的配置项；
			如果配置项不存在，
			根据默认值创建一个新的配置项并返回
		******************************************/
		template<class T>
		static typename ConfigVar<T>::ptr Lookup(const std::string& name, 
			const T& default_value, 
			const std::string& description = "")
		{
			auto tmp = Lookup<T>(name);
			if (tmp != nullptr)
			{
				NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "Lookup name=" << name << " exists";
				return tmp;
			}
			std::regex pattern("^[a-zA-Z0-9\\._]+$");
			if (!std::regex_match(name, pattern))
			{
				NILESTHUMP_LOG_ERROR(NILESTHUMP_LOG_ROOT()) << "Lookup name invalid " << name;
				throw std::invalid_argument(name);
			}
			typename ConfigVar<T>::ptr v = std::make_shared<ConfigVar<T>>(name, default_value, description);
			s_datas[name] = v;
			return v;
		}

		/********************************************
			根据名称查找配置项，返回配置项指针，
			如果没有找到返回nullptr
			注意：此函数不会创建配置项
		********************************************/
		template<class T>
		static typename ConfigVar<T>::ptr Lookup(const std::string& name)
		{
			auto it = s_datas.find(name);
			if (it == s_datas.end())
			{
				return nullptr;
			}
			return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
		}
	private:
		static ConfigVarMap s_datas;
	};

}