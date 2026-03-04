# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.15)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (GameProjectServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 Boost 库的路径,请根据实际安装路径进行修改
set(Boost_ROOT "D:/boost_1_90_0")
set(Boost_LIB_DIR ${Boost_ROOT}/lib32-msvc-14.3)

find_package(Boost 1.90 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(${PROJECT_SOURCE_DIR}/source)

include_directories(${PROJECT_SOURCE_DIR}/include)

# link_libraries(${LIB_PATH}/GameProjectServer)
add_executable(test tests/test.cpp)
target_link_libraries(test PUBLIC GameProjectServer)

# link_libraries(${LIB_PATH}/GameProjectServer)
add_executable(test_config tests/test_config.cpp)
target_link_libraries(test_config PUBLIC GameProjectServer)

