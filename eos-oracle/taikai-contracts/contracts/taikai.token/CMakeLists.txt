
add_contract(taikai.token taikai.token ${CMAKE_CURRENT_SOURCE_DIR}/src/taikai.token.cpp)

target_include_directories(taikai.token.wasm
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(taikai.token.wasm
   PROPERTIES
   BINARY_DIR "${CMAKE_BINARY_DIR}"
)


