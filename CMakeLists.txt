# DCMTK Demo Project

# declare project
project (NBStrucuredReport)

# minimum CMake version required
cmake_minimum_required (VERSION 3.5)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")

set (source_dir "${PROJECT_SOURCE_DIR}/src/")
file (GLOB source_files "${source_dir}/*.cpp")

# specify DCMTK's (default) installation directory
#SET(DCMTK_DIRECTORY "/home/david/Documentos/dcmtk-3.6.5-install" CACHE PATH "Directory where DCMTK library is installed.")
SET(DCMTK_DIRECTORY "/usr/local" CACHE PATH "Directory where DCMTK library is installed.")
#SET(JSON_DIRECTORY "/home/david/Documentos/json-develop-install" CACHE PATH "Directory where JSON library is installed.")
SET(JSON_DIRECTORY "/home/david/Documentos/json-develop-install" CACHE PATH "Directory where JSON library is installed.")

# approach #1: use FIND_PACKAGE() to search for installed DCMTK
# FIND_PACKAGE(DCMTK REQUIRED CONFIG PATHS ${DCMTK_DIRECTORY} NO_DEFAULT_PATH)
FIND_PACKAGE(DCMTK REQUIRED)
# approach #2: include DCMTK's CMake configuration directly
# INCLUDE(${DCMTK_DIRECTORY}/usr/local/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/dcmtk/DCMTKConfig.cmake)
# INCLUDE(${DCMTK_DIRECTORY}/usr/local/lib/cmake/dcmtk/DCMTKConfig.cmake)

find_package(nlohmann_json 3.7.0 REQUIRED)
# target_link_libraries(configuration PRIVATE nlohmann_json::nlohmann_json)

# declare include directories
# INCLUDE_DIRECTORIES(${DCMTK_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${DCMTK_DIRECTORY}/include)
LINK_DIRECTORIES(${DCMTK_DIRECTORY}/lib)
LINK_DIRECTORIES(${DCMTK_DIRECTORY}/bin)

INCLUDE_DIRECTORIES(${JSON_DIRECTORY}/include)
LINK_DIRECTORIES(${JSON_DIRECTORY}/lib)

# declare executable and link required libraries
ADD_EXECUTABLE(NBStrucuredReport Main.cc)
TARGET_LINK_LIBRARIES(NBStrucuredReport ofstd dcmdata dcmsr dcmdata nlohmann_json::nlohmann_json)
