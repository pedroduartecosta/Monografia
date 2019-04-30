cmake_minimum_required(VERSION 3.5)
project( TaikaiTestContracts )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../contracts/taikai.fund/include)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

enable_testing()

include_directories(${CMAKE_BINARY_DIR})

file(GLOB UNIT_TESTS "*.cpp")

add_eosio_test( unit_test ${UNIT_TESTS} )
