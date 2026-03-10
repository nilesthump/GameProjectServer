#pragma once

#include <memory>
#include <string>
#include <exception>
#include <functional>

#include <map>
#include <vector>
#include <set>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include <sstream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include "Log.h"
#include "yaml-cpp/yaml.h"

#ifdef _MSC_VER
#ifdef NST_LIB_EXPORTS
#define CONFIG_API __declspec(dllexport)
#else
#define CONFIG_API __declspec(dllimport)
#endif
#elif define(__GNUC__)
#define CONFIG_API __attribute__((visibility("default")))
#else
#define CONFIG_API
#endif

namespace GameProjectServer
{
	class ConfigVarBase
	{
	public:
		using ptr = std::shared_ptr<ConfigVarBase>;
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
		virtual std::string getTypeName() const = 0;	//获取配置项类型的名称
	protected:
		std::string m_name;
		std::string m_description;
	};

	//F from_type, T to_type
	template<class F, class T>
	class LexicalCast
	{
	public:
		T operator()(const F& v)
		{
			return boost::lexical_cast<T>(v);
		}
	};

	//STL偏特化

	//string - vector
	template<class T>
	class LexicalCast<std::string, std::vector<T>>
	{
	public:
		std::vector<T> operator()(const std::string& v) noexcept
		{
			YAML::Node node = YAML::Load(v);
			typename std::vector<T> vec;
			std::stringstream ss;
			for (size_t i = 0; i < node.size(); ++i)
			{
				ss.str("");
				ss << node[i];
				vec.push_back(LexicalCast<std::string, T>()(ss.str()));
			}
			return vec;
		}
	};

	template<class T>
	class LexicalCast<std::vector<T>, std::string>
	{
	public:
		std::string operator()(const std::vector<T>& v) noexcept
		{
			YAML::Node node;
			for (auto& i : v)
			{
				node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
			}
			std::stringstream ss;
			ss << node;
			return ss.str();
		}
	};

	//string - list
	template<class T>
	class LexicalCast<std::string, std::list<T>>
	{
	public:
		std::list<T> operator()(const std::string& v) noexcept
		{
			YAML::Node node = YAML::Load(v);
			typename std::list<T> lst;
			std::stringstream ss;
			for (size_t i = 0; i < node.size(); ++i)
			{
				ss.str("");
				ss << node[i];
				lst.push_back(LexicalCast<std::string, T>()(ss.str()));
			}
			return lst;
		}
	};

	template<class T>
	class LexicalCast<std::list<T>, std::string>
	{
	public:
		std::string operator()(const std::list<T>& v) noexcept
		{
			YAML::Node node;
			for (auto& i : v)
			{
				node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
			}
			std::stringstream ss;
			ss << node;
			return ss.str();
		}
	};

	//string - set
	template<class T>
	class LexicalCast<std::string, std::set<T>>
	{
	public:
		std::set<T> operator()(const std::string& v) noexcept
		{
			YAML::Node node = YAML::Load(v);
			typename std::set<T> st;
			std::stringstream ss;
			for (size_t i = 0; i < node.size(); ++i)
			{
				ss.str("");
				ss << node[i];
				st.insert(LexicalCast<std::string, T>()(ss.str()));
			}
			return st;
		}
	};

	template<class T>
	class LexicalCast<std::set<T>, std::string>
	{
	public:
		std::string operator()(const std::set<T>& v) noexcept
		{
			YAML::Node node;
			for (auto& i : v)
			{
				node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
			}
			std::stringstream ss;
			ss << node;
			return ss.str();
		}
	};

	//string - unordered_set
	template<class T>
	class LexicalCast<std::string, std::unordered_set<T>>
	{
	public:
		std::unordered_set<T> operator()(const std::string& v) noexcept
		{
			YAML::Node node = YAML::Load(v);
			typename std::unordered_set<T> un_st;
			std::stringstream ss;
			for (size_t i = 0; i < node.size(); ++i)
			{
				ss.str("");
				ss << node[i];
				un_st.insert(LexicalCast<std::string, T>()(ss.str()));
			}
			return un_st;
		}
	};

	template<class T>
	class LexicalCast<std::unordered_set<T>, std::string>
	{
	public:
		std::string operator()(const std::unordered_set<T>& v) noexcept
		{
			YAML::Node node;
			for (auto& i : v)
			{
				node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
			}
			std::stringstream ss;
			ss << node;
			return ss.str();
		}
	};

	//string - map
	template<class T>
	class LexicalCast<std::string, std::map<std::string, T>>
	{
	public:
		std::map<std::string, T> operator()(const std::string& v) noexcept
		{
			YAML::Node node = YAML::Load(v);
			typename std::map<std::string, T> mp;
			std::stringstream ss;
			for (auto it = node.begin(); it != node.end(); ++it)
			{
				ss.str("");
				ss << it->second;
				mp.insert(std::make_pair(it->first.Scalar(),
					LexicalCast<std::string, T>()(ss.str())));
			}
			return mp;
		}
	};

	template<class T>
	class LexicalCast<std::map<std::string, T>, std::string>
	{
	public:
		std::string operator()(const std::map<std::string, T>& v) noexcept
		{
			YAML::Node node;
			for (auto& i : v)
			{
				node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
			}
			std::stringstream ss;
			ss << node;
			return ss.str();
		}
	};

	//string - unordered_map
	template<class T>
	class LexicalCast<std::string, std::unordered_map<std::string, T>>
	{
	public:
		std::unordered_map<std::string, T> operator()(const std::string& v) noexcept
		{
			YAML::Node node = YAML::Load(v);
			typename std::unordered_map<std::string, T> un_mp;
			std::stringstream ss;
			for (auto it = node.begin(); it != node.end(); ++it)
			{
				ss.str("");
				ss << it->second;
				un_mp.insert(std::make_pair(it->first.Scalar(),
					LexicalCast<std::string, T>()(ss.str())));
			}
			return un_mp;
		}
	};

	template<class T>
	class LexicalCast<std::unordered_map<std::string, T>, std::string>
	{
	public:
		std::string operator()(const std::unordered_map<std::string, T>& v) noexcept
		{
			YAML::Node node;
			for (auto& i : v)
			{
				node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
			}
			std::stringstream ss;
			ss << node;
			return ss.str();
		}
	};

	//FromStr T operator()(const std::string& str)
	//ToStr std::string operator()(const T& v)
	template<class T, class FromStr = LexicalCast<std::string, T> 
	, class ToStr = LexicalCast<T, std::string>>
	class ConfigVar : public ConfigVarBase
	{
	public:
		using ptr = std::shared_ptr<ConfigVar>;
		using on_change_cb = std::function<void(const T& old_value, const T& new_value)>;

		ConfigVar(const std::string& name, const T& default_value, const std::string& description = "")
			: ConfigVarBase(name, description)
			, m_val(default_value)
		{
		}
		std::string toString() override
		{
			try
			{
				//return boost::lexical_cast<std::string>(m_val);
				return ToStr()(m_val);
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
				//m_val = boost::lexical_cast<T>(val);
				setValue(FromStr()(val));
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
		void setValue(const T& v) 
		{
			if (v == m_val)
			{
				return;
			}
			for (auto& i : m_cbs)
			{
				i.second(m_val, v);
			}
			m_val = v;
		}
		std::string getTypeName() const override { return typeid(T).name(); }

		void addListener(uint64_t key, on_change_cb cb) noexcept
		{
			if (m_cbs.find(key) != m_cbs.end())
			{
				NILESTHUMP_LOG_ERROR(NILESTHUMP_LOG_ROOT()) <<
					"ConfigVar::addListener failed, key=" << key << " already exists";
				return;
			}
			m_cbs[key] = cb;
		}

		void delListener(uint64_t key) noexcept
		{
			m_cbs.erase(key);
		}

		on_change_cb getListener(uint64_t key) noexcept
		{
			auto it = m_cbs.find(key);
			return it == m_cbs.end() ? nullptr : it->second;
		}

		void clearListener() noexcept
		{
			m_cbs.clear();
		}
	private:
		T m_val;
		//变更回调函数组，当配置项变更时，调用回调函数通知外部
		// uint64_t key, 要求唯一，一般用hash
		std::map<uint64_t, on_change_cb> m_cbs;
	};

	class CONFIG_API Config
	{
	public:
		using ConfigVarMap = std::map<std::string, ConfigVarBase::ptr>;

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
			auto it = s_datas.find(name);
			if (it != s_datas.end())
			{
				auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
				if (tmp) 
				{
					NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "Lookup name=" << name << " exists";
					return tmp;
				}
				else
				{
					NILESTHUMP_LOG_ERROR(NILESTHUMP_LOG_ROOT()) << "Lookup name=" << name <<
						" exists but type not " << typeid(T).name() <<
						" real_type=" << it->second->getTypeName() << " " << it->second->toString();
					return nullptr;
				}
			}
			auto tmp = Lookup<T>(name);
			if (tmp != nullptr)
			{
				NILESTHUMP_LOG_INFO(NILESTHUMP_LOG_ROOT()) << "Lookup name=" << name << " exists";
				return tmp;
			}
			boost::regex pattern("^[a-zA-Z0-9\\._]*$");
			if (!boost::regex_match(name, pattern))
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

		static void LoadFromYaml(const YAML::Node& root);
		static inline ConfigVarBase::ptr LookupBase(const std::string& name);
	private:
		static inline ConfigVarMap s_datas;
	};

}