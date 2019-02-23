cmake_minimum_required(VERSION 3.5)
project(hipoTests)
set(VERSION 1.0.0)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})

set(CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${PROJECT_SOURCE_DIR}/cmake
   ${HIPO_TOOLS}/lib/hipo)

include_directories("src")

# e1d
add_executable(rootTest src/rootTest.cxx)
target_link_libraries(rootTest ${ROOT_LIBRARIES})
install(TARGETS rootTest RUNTIME DESTINATION bin)
