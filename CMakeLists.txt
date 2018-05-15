cmake_minimum_required(VERSION 3.5)
project(stuff_getter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# Download and unpack googletest at configure time
configure_file(CMakeLists.gtest.txt googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# find_package(CLI11 CONFIG)

add_executable(main src/main.cpp)
add_executable(cli src/cli.cpp)

#-------------------
# Unit Testing
#-------------------
enable_testing()

set(GTEST_SRC_FILES
	tests/test_dir_poll.cc
)

foreach(_file ${GTEST_SRC_FILES})
    get_filename_component(_name ${_file} NAME_WE)
    add_executable(${_name} ${_file})
    target_link_libraries(${_name} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})
    add_test(${_name} ${_name})
    set_tests_properties(${_name} PROPERTIES TIMEOUT 70)
endforeach()
