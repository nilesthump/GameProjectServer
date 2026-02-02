# CMakeList.txt: GameProjectServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将源代码添加到此项目的库中
set(LIB_SRC
    Log.cpp
    Log.h
)

# 创建共享库
add_library(GameProjectServer SHARED ${LIB_SRC})

# 创建静态库
add_library(GameProjectServer_static STATIC ${LIB_SRC})
SET_TARGET_PROPERTIES(GameProjectServer_static PROPERTIES OUTPUT_NAME "GameProjectServer")

# 设置包含目录
target_include_directories(GameProjectServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GameProjectServer_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 设置 C++ 标准
set_target_properties(GameProjectServer PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

set_target_properties(GameProjectServer_static PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
