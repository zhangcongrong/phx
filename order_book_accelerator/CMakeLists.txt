
project(order_book_ext) # Replace 'my_project' with the name of your project
cmake_minimum_required(VERSION 3.15...3.27)
find_package(Python 3.9 COMPONENTS Interpreter Development.Module REQUIRED)


# Next configure CMake to perform an optimized release build by default unless another build type is specified.
# Without this addition, binding code may run slowly and produce large binaries.
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Detect the installed nanobind package and import it into CMake
execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

# build the extensions
nanobind_add_module(order_book_ext order_book_ext.cpp)

# Install directive for scikit-build-core
install(TARGETS order_book_ext LIBRARY DESTINATION order_book_accelerator)