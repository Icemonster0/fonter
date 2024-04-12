cmake_minimum_required(VERSION 3.8)

add_executable(test_texture test_texture.cpp
    ../src/texture.cpp
    ../src/math.cpp
    ../src/error_log.cpp
)

add_executable(test_loader test_loader.cpp
    ../src/texture.cpp
    ../src/math.cpp
    ../src/font.cpp
    ../src/error_log.cpp
    ../src/loader.cpp
    ../src/symbol.cpp
)
add_executable(test_glypher test_glypher.cpp
    ../src/texture.cpp
    ../src/math.cpp
    ../src/font.cpp
    ../src/error_log.cpp
    ../src/loader.cpp
    ../src/symbol.cpp
    ../src/glyph.cpp
    ../src/glypher.cpp
)
