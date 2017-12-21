cmake_minimum_required(VERSION 2.8.2)
project(NR_MCPP)

include(CTest)

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

# When using CMake 2.8.11 or later, header path dependencies
# are automatically added to the gtest and gmock targets.
# For earlier CMake versions, we have to explicitly add the
# required directories to the header search path ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("${PROJECT_SOURCE_DIR}/minimization"
                    "${PROJECT_SOURCE_DIR}/SIMD")

# Trivial example using gtest 
add_executable(minimization 
               main.cpp 
               minimization/downhill_simplex.cpp 
               minimization/downhill_simplex_m.cpp 
               minimization/test_downhill_simplex.cpp)

add_executable(simd 
               main.cpp 
               simd/SIMD.cpp
               simd/test_SIMD.cpp)

target_link_libraries(minimization gtest)
target_link_libraries(simd gtest)

add_test(NAME minimization COMMAND minimization)
add_test(NAME simd COMMAND simd)
