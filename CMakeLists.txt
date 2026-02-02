# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.10)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GameProjectServer")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -03 -g -std=c++11 -Wall")

set(LIB_SRC
	GameProjectServer/Log.cpp
	GameProjectServer/Log.h
	)

add_library(GameProjectServer SHARED ${LIB_SRC})
#add_library(GameProjectServer_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (GameProjectServer_static PROPERTIES OUTPUT_NAME "GameProjectServer")

add_executable(test tests/test.cpp)
add_dependencies(test GameProjectServer)
target_link_libraries(test GameProjectServer)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 包含子项目。
#add_subdirectory ("GameProjectServer")
