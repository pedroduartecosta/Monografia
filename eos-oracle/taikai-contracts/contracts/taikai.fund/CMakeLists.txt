
add_contract(taikai.fund taikai.fund ${CMAKE_CURRENT_SOURCE_DIR}/src/taikai.fund.cpp)

target_include_directories(taikai.fund.wasm
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(taikai.fund.wasm
   PROPERTIES
   BINARY_DIR "${CMAKE_BINARY_DIR}"
)