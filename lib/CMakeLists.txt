cmake_minimum_required(VERSION 3.16)
project(Libs LANGUAGES CXX)

set(KAFKA_LIB_DIR ${PROJECT_SOURCE_DIR}/kafka)
set(ELASTIC_CLIENT_DIR ${PROJECT_SOURCE_DIR}/elasticlient)
set(RDKAFKA_LIB_DIR ${PROJECT_SOURCE_DIR}/rdkafka)
set(STEGO_LIB_DIR ${PROJECT_SOURCE_DIR}/stego)

add_subdirectory(${STEGO_LIB_DIR})
add_subdirectory(${ELASTIC_CLIENT_DIR})
add_subdirectory(${KAFKA_LIB_DIR})
add_subdirectory(${RDKAFKA_LIB_DIR})

