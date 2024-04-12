cmake_minimum_required(VERSION 3.8)
project(fonter VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(libs_module STATIC
    src/lib/clom2.cpp
    src/lib/clom.hpp
    src/lib/glm.hpp
    src/lib/stb.cpp
    src/lib/stb.hpp
)

target_compile_options(libs_module BEFORE PUBLIC -O3)

add_library(main_module STATIC
    src/compositor.cpp
    src/compositor.hpp
    src/error_log.hpp
    src/error_log.cpp
    src/font.cpp
    src/font.hpp
    src/glyph.cpp
    src/glyph.hpp
    src/glypher.cpp
    src/glypher.hpp
    src/fonter.cpp
    src/fonter.hpp
    src/loader.cpp
    src/loader.hpp
    src/math.cpp
    src/math.hpp
    src/rules.hpp
    src/symbol.cpp
    src/symbol.hpp
    src/texture.cpp
    src/texture.hpp
    src/user_config.cpp
    src/user_config.hpp
)

target_compile_options(main_module BEFORE PUBLIC -O3)
target_link_libraries(main_module libs_module)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} main_module)

add_subdirectory(tests)

cmake_policy(SET CMP0079 NEW)
target_link_libraries(test_texture libs_module)
target_link_libraries(test_loader libs_module)
target_link_libraries(test_glypher libs_module)
