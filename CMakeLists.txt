cmake_minimum_required(VERSION 3.22)
project(PredictionTrie)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/danila/Qt/6.2.4/macos/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(PredictionTrie main.cpp PredictionTrie.cpp PredictionTrie.h TextEditor.cpp TextEditor.h)
target_link_libraries(PredictionTrie
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        )

