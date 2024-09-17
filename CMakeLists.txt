cmake_minimum_required(VERSION 3.15.1)
project(Beast-core)

file(COPY "${CMAKE_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}")


file(GLOB_RECURSE SOURCES_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.c"
)
add_library(Beast-core SHARED ${SOURCES_FILES})
target_include_directories(Beast-core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/headers"
    "${CMAKE_CURRENT_SOURCE_DIR}/sources"
)
target_link_libraries(Beast-core PUBLIC user32 Kernel32)
set_target_properties(Beast-core PROPERTIES OUTPUT_NAME "Beast-core" PREFIX "" SUFFIX ".dll" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_executable(example sandbox/main.cpp)
target_link_libraries(example PRIVATE Beast-core d3d11 D3DCompiler) 
set_target_properties(example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})