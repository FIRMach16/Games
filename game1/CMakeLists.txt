cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(SOURCES
    src/main.cpp
    src/GameController.cpp
    src/GameView.cpp
    src/GameModel.cpp)
add_executable(tictactoe ${SOURCES})
target_compile_features(tictactoe PRIVATE cxx_std_17)
target_link_libraries(tictactoe PRIVATE SFML::Graphics)
