cmake_minimum_required(VERSION 3.0.2)
project(finite-automatons VERSION 4.9.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# Define an imported library target
add_library(antlr4 STATIC IMPORTED) # Or SHARED

# Set the location of the library file
set_property(TARGET antlr4 PROPERTY IMPORTED_LOCATION "antlr4/libantlr4-runtime.a")

# Specify the include directory so the compiler can find headers
target_include_directories(antlr4 INTERFACE antlr4/headers)

# Link it to your executable
add_executable(nfa
        include/cli_parser.h include/DFA.h include/DFAStates.h include/NFA.h
        src/cli_parser.cpp src/DFA.cpp src/main.cpp src/NFA.cpp src/ENFA.cpp
        REParser/RegExBaseListener.cpp REParser/RegExLexer.cpp REParser/RegExParser.cpp REParser/RegExListener.cpp
)
target_link_libraries(nfa PRIVATE antlr4)

include_directories(include)
include_directories(REParser)
#include_directories(antlr4/runtime/src "antlr4/runtime/src/*" "antlr4/runtime/src/tree/*")
