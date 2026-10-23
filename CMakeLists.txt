cmake_minimum_required(VERSION 3.20)

project(text_editor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(piece_table STATIC
    src/PieceTable.cpp
)

target_include_directories(piece_table
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.18.0
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(googletest)
    add_executable(PieceTableTests
    tests/PieceTableTest.cpp
    )

    target_link_libraries(PieceTableTests
        PRIVATE
            piece_table
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(PieceTableTests)
endif()