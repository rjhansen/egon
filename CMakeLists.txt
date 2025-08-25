cmake_minimum_required(VERSION 3.30.0 FATAL_ERROR)
enable_testing()

# C++ Language-specific variables, setting build env constraints
set(CMAKE_CXX_STANDARD 23 REQUIRED)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_C_STANDARD 11 REQUIRED)
set(CMAKE_C_EXTENSIONS Off)
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/rjhansen/egon")

project(egon
    VERSION 1.0.0
    DESCRIPTION "Imports Proton Mail certificates into GnuPG."
    LANGUAGES CXX C
    HOMEPAGE_URL CMAKE_PROJECT_HOMEPAGE_URL)

add_subdirectory(src)

if (UNIX OR APPLE)
  include(GNUInstallDirs)
endif()

install(TARGETS egon)