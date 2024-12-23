cmake_minimum_required(VERSION 3.28)
project(SFML_Kinematics VERSION 0.1.0 LANGUAGES CXX)

# Enable test
include(CTest)
enable_testing()

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Create a list of compiler warning flags for GCC/Clang
# if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#     add_compile_options(-Wall -Wextra -Wpedantic -Werror)
# endif()

# # Add MSVC specific warnings (optional)
# if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#     add_compile_options(/W4 /WX)  # Equivalent to high warning level and treat warnings as errors
# endif()

# Specify output directories based on the build type
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

# Glob for source files
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Include SFML for fetch content
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Create Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} 
PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics)

# Packaging configuration
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)