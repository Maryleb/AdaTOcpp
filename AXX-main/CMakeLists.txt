cmake_minimum_required(VERSION 3.8)
project(Ada_AST_builder
    VERSION 0.1.0
    DESCRIPTION "Abstract syntax tree builder for Ada language"
    LANGUAGES CXX)

include_directories(include)

set(exename axx)

set(tokenlib src/axx/Token.cpp)
set(astlib src/axx/ASTNode.cpp src/axx/AST.cpp)
set(lexlib src/axx/Lexer.cpp src/axx/LexerStates.cpp src/axx/FileData.cpp)
set(parslib src/axx/Parser.cpp)
set(semlib src/axx/SemanticAnalyzer.cpp src/axx/SemanticVisitor.cpp src/axx/Symbol.cpp)
set(codegenlib src/axx/CodeGenerator.cpp src/axx/CodeEmittingNodeVisitor.cpp)

add_library(token STATIC ${tokenlib})
add_library(ast STATIC ${astlib})
add_library(lexer STATIC ${lexlib})
add_library(parser STATIC ${parslib})
add_library(semantic STATIC ${semlib})
add_library(codegen STATIC ${codegenlib})

target_link_libraries(lexer token)
target_link_libraries(parser lexer ast token)

set(libs lexer parser semantic codegen)

add_executable(${exename} main.cpp)
set_property(TARGET ${exename} PROPERTY CXX_STANDARD 17)

if(MSVC)
  target_compile_options(${exename} PRIVATE /W4 /WX)
else()
  target_compile_options(${exename} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

file(COPY example_script.ads DESTINATION .)

target_link_libraries(${exename} ${libs})
