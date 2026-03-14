#pragma once
#include <memory>

#ifdef _MSC_VER
#ifdef NST_LIB_EXPORTS
#define SINGLETON_API __declspec(dllexport)
#else
#define SINGLETON_API __declspec(dllimport)
#endif
#elif define(__GNUC__)
#define SINGLETON_API __attribute__((visibility("default")))
#else
#define SINGLETON_API
#endif

namespace GameProjectServer
{

	template<class T, class X = void, int N = 0>
	class Singleton
	{
	public:
		static T* GetInstance()
		{
			static T v;
			return &v;
		}
	};

	template<class T, class X = void, int N = 0>
	class SINGLETON_API SingletonPtr
	{
		public:
		static std::shared_ptr<T> GetInstance()
		{
			static std::shared_ptr<T> v(new T);
			return v;
		}
	};
}