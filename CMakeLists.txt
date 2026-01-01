cmake_minimum_required(VERSION 3.21.0)

project(
  SunnyLand
  LANGUAGES C CXX
  VERSION 0.1.0
)

# 指定搜索路径（用于find_package在编译期查找库）
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/prebuilt)

# option(BUILD_SHARED_LIBS "依赖库默认编译为动态库" OFF)

# 编译器设置（C++标准、编译选项等）
include(cmake/CompilerSettings.cmake)

# 运行时路径配置（RPATH）
include(cmake/RuntimePath.cmake)

# 依赖管理
include(cmake/Dependencies.cmake)

# 调用依赖配置函数（定义在Dependencies.cmake中）
setup_project_dependencies()

set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加可执行文件
add_executable(
        ${TARGET}
        src/main.cpp
        src/engine/core/Time.cpp
        src/engine/core/GameApp.cpp
        src/engine/resource/ResourceManager.cpp
        src/engine/resource/TextureManager.cpp
        src/engine/resource/AudioManager.cpp
        src/engine/resource/FontManager.cpp
)

# 链接库
target_link_libraries(
  ${TARGET}
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_mixer::SDL3_mixer
  SDL3_ttf::SDL3_ttf
  glm::glm
  nlohmann_json::nlohmann_json
  spdlog::spdlog
)


# ============================================
# 应用配置
# ============================================

# 设置编译选项（定义在CompilerSettings.cmake中）
setup_compiler_options(${TARGET})