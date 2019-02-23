cmake_minimum_required(VERSION 3.5)
project(hipoTests)
set(VERSION 1.0.0)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})

set(CMAKE_MODULE_PATH
   ${CMAKE_MODULE_PATH}
   ${PROJECT_SOURCE_DIR}/cmake
 $ENV{HIPO_TOOLS}/lib/hipo)

include_directories("src")

# rootTest
add_executable(rootTest src/rootTest.cxx)
target_link_libraries(rootTest ${ROOT_LIBRARIES})
install(TARGETS rootTest RUNTIME DESTINATION bin)

find_package(Java)
if(Java_FOUND)
  include(UseJava)
  set(CMAKE_JAVA_INCLUDE_PATH_FINAL "$ENV{CLAS12LIBS}/*")
  set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.8" "-target" "1.8")
  add_jar(hipo3test
    SOURCES src/hipo3test.java
    )
  add_jar(hipo4test
    SOURCES src/hipo4test.java
    )
endif()
