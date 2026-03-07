# CMakeList.txt: GameProjectServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

cmake_minimum_required (VERSION 3.15)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# 将源代码添加到此项目的库中

aux_source_directory(. LIB_SRC)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

if(WIN32)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# 创建共享库
add_definitions(-DNST_LIB_EXPORTS)
add_library(GameProjectServer SHARED ${LIB_SRC})
target_include_directories(GameProjectServer PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(GameProjectServer PUBLIC ${Boost_LIBRARIES} Boost::boost yaml-cpp::yaml-cpp)