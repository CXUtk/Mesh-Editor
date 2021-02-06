# CMakeList.txt: CMakeTest 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project (CMakeTest VERSION 1.0)

file (GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# 将源代码添加到此项目的可执行文件。
add_executable (CMakeTest ${SRC_FILES})

set_target_properties (CMakeTest PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      )
  

target_include_directories (CMakeTest PUBLIC "${PROJECT_SOURCE_DIR}/deps")

include_directories (${PROJECT_SOURCE_DIR}/src)
include_directories (${PROJECT_SOURCE_DIR}/deps)

FIND_PACKAGE (OpenGL REQUIRED)

add_subdirectory ("deps/glad")
add_subdirectory ("deps/lodepng")


#target_link_directories (CMakeTest PRIVATE ${PROJECT_SOURCE_DIR}/deps/GLFW/lib)
target_link_libraries (CMakeTest PRIVATE "${PROJECT_SOURCE_DIR}/deps/GLFW/lib/glfw3.lib")
target_link_libraries (CMakeTest PRIVATE glad)
target_link_libraries (CMakeTest PRIVATE lodepng)

add_custom_command(TARGET CMakeTest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${PROJECT_SOURCE_DIR}/Resources  ${PROJECT_BINARY_DIR}/Resources)


# TODO: 如有需要，请添加测试并安装目标。
