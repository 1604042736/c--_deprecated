cmake_minimum_required(VERSION 3.23)
project(CQQ)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/ASTs/)
include_directories(${PROJECT_SOURCE_DIR}/Pegen/)

#设置Pegen
aux_source_directory(Pegen PEGEN_SRC)
add_executable(pegen ${PEGEN_SRC})