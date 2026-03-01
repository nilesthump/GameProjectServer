# CMakeList.txt: GameProjectServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# 将源代码添加到此项目的库中
set(LIB_SRC
    Log.cpp
    Log.h
    Util.cpp
    Util.h
    Singleton.h
    Config.cpp
    Config.h
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
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

set_target_properties(GameProjectServer_static PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 为共享库启用热重载支持
if(MSVC)
    # 为调试配置启用编辑并继续
    target_compile_options(GameProjectServer PRIVATE
        $<$<CONFIG:Debug>:/ZI>
        $<$<CONFIG:RelWithDebInfo>:/ZI>
    )
    # 设置链接选项以支持热重载
    target_link_options(GameProjectServer PRIVATE
        $<$<CONFIG:Debug>:/INCREMENTAL>
        $<$<CONFIG:RelWithDebInfo>:/INCREMENTAL>
    )
endif()
