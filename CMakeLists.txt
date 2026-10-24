cmake_minimum_required(VERSION 3.28)

project(FunWithReflection LANGUAGES C CXX)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include (FetchContent)
# Grab reflect-cpp if it's not already installed

# Recommend these settings for reflectcpp.
# If you're building it to install you may
# also want to enable some of their other
# reflection formats -- see the options
# in their CMakeLists.txt
set(REFLECTCPP_USE_CPP26_REFLECTION "ON" CACHE STRING "Use C++26 Reflection" FORCE)
set(REFLECTCPP_USE_STD_EXPECTED "ON" CACHE STRING "Use std::expected rather than built in result type" FORCE)
FetchContent_Declare(
  reflectcpp
  GIT_REPOSITORY https://github.com/munich-data-engineering/reflect-cpp.git
  GIT_TAG v0.26.2
  FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(reflectcpp)

# Set up our C++ interface library -- the heart of the operation

add_library(SerializerCore INTERFACE)

target_include_directories(SerializerCore INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# target_link_libaries will propagate our reflectcpp dependency
# to anything using this library, so we don't have to specify
# it again.

target_link_libraries(SerializerCore INTERFACE reflectcpp::reflectcpp)

# Build our demo library and program to output our demo
# header file

add_library(DemoSerializer STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Demo.cpp"
)

# We use some includes in demo for Demo.cpp
target_include_directories(DemoSerializer PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/demo"
)

# And our serializer core
target_link_libraries(DemoSerializer PUBLIC SerializerCore)

# Set up our program to emit our header
add_executable(demo_header
  "${CMAKE_CURRENT_SOURCE_DIR}/demo/demo_header.cpp"
)

target_link_libraries(demo_header PUBLIC SerializerCore)
target_include_directories(demo_header PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/demo")

# Add a dependency to generate our demo library header prior trying to compile
# the C demo. If we were packaging the library, we could just generate this
# file and package it along with the library
set(DEMO_LIBRARY_HEADER "${CMAKE_CURRENT_BINARY_DIR}/demo_header.h")
add_custom_command(
  OUTPUT "${DEMO_LIBRARY_HEADER}"
  COMMAND demo_header "${DEMO_LIBRARY_HEADER}"
  DEPENDS demo_header
  COMMENT "Generating demo header"
  VERBATIM
)

# Add our header dependency to our C Demo
add_executable(c_demo ${CMAKE_CURRENT_SOURCE_DIR}/demo/c_demo.c "${DEMO_LIBRARY_HEADER}")

target_link_libraries(c_demo PUBLIC DemoSerializer)
target_include_directories(c_demo PUBLIC
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/demo"
)
