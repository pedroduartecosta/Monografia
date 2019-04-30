cmake_minimum_required(VERSION 3.5)
project( TaikaiContracts )

set(EOSIO_CONTRACT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/eosio.contracts/)
set(EOSIO_TOKEN_CONTRACT_DIR ${EOSIO_CONTRACT_DIR}/eosio.token/)
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_EXTENSIONS ON )
set( CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(EOSIO_CDT_VERSION_MIN "1.4.1")
set(EOSIO_CDT_VERSION_SOFT_MAX "1.5")

find_package(eosio.cdt)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   set(TEST_BUILD_TYPE "Debug")
   set(CMAKE_BUILD_TYPE "Release")
else()
   set(TEST_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif()

include(GNUInstallDirs)
include(EosioWasmToolchain)
add_subdirectory(contracts)
include(ExternalProject)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   set(TEST_BUILD_TYPE "Debug")
   set(CMAKE_BUILD_TYPE "Release")
else()
   set(TEST_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif()

message(STATUS "CMAKE  DCMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE  DCMAKE_FRAMEWORK_PATH = ${DCMAKE_FRAMEWORK_PATH}")
message(STATUS "CMAKE  DCMAKE_MODULE_PATH = ${DCMAKE_MODULE_PATH}")
message(STATUS "CMAKE  EOSIO_ROOT = ${EOSIO_ROOT}")
message(STATUS "CMAKE  LLVM_DIR = ${LLVM_DIR}")


#ExternalProject_Add(tests
#               CMAKE_ARGS -DCMAKE_BUILD_TYPE=${TEST_BUILD_TYPE} -DCMAKE_FRAMEWORK_PATH=${TEST_FRAMEWORK_PATH} -DCMAKE_MODULE_PATH=${TEST_MODULE_PATH} -DEOSIO_ROOT=${EOSIO_ROOT} -DLLVM_DIR=${LLVM_DIR}
#               SOURCE_DIR ${CMAKE_SOURCE_DIR}/tests
#               BINARY_DIR ${CMAKE_BINARY_DIR}/tests)

message(STATUS "Taikai Token Development using  eosio.cdt version ${EOSIO_CDT_VERSION}")