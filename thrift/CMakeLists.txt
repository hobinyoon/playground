cmake_minimum_required(VERSION 3.16)
project(ThriftExample)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-fsanitize=address)
add_compile_options(-Wall)
add_link_options(-fsanitize=address)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

list(APPEND IDL_COMPILED_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/gen-cpp/CalculatorService.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/gen-cpp/calculator_constants.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/gen-cpp/calculator_types.cpp
)

add_custom_command(
  OUTPUT ${IDL_COMPILED_FILES}
  COMMAND thrift --gen cpp ${CMAKE_CURRENT_SOURCE_DIR}/calculator.thrift
  COMMENT "Generating Thrift stubs")

set_source_files_properties(${IDL_COMPILED_FILES}
                            PROPERTIES GENERATED TRUE)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(calculator_server
    ${IDL_COMPILED_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/calculator_server.cpp
)
target_link_libraries(calculator_server thrift)

add_executable(calculator_client
    ${IDL_COMPILED_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/calculator_client.cpp
)
target_link_libraries(calculator_client thrift)
