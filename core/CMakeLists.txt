cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

colormsg(_HIBLUE_ "Configuring VIXAL-Core:")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir
set(CMAKE_CXX_STANDARD 14)

# CMake useful variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Set the name of your project here
project("VIXAL")

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Core targets configuration
string(TOLOWER "${PROJECT_NAME}" PROJECTNAMEL)
#this command will update parent scope variable
set(VIXAL_CORE_TARGET ${PROJECTNAMEL}-core PARENT_SCOPE)
set(VIXAL_CORE_TARGET ${PROJECTNAMEL}-core)

include(Common)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

## Git (and its revision)
find_package(Git QUIET) # if we don't find git or FindGit.cmake is not on the system we ignore it.
## GetGitRevisionDescription module to retreive branch and revision information from Git
## Starting with Git 1.9 the module will be part of official cMake distribution, until then it has to be
## part of the application
## The Git module will trigger a reconfiguration for each pull that will bring a new revision on the local repository
#set (VCS_REVISION "-1")
#if(GIT_FOUND)
#    include(GetGitRevisionDescription)
#    get_git_head_revision(GIT_REFSPEC GIT_SHA1)
#    message(STATUS "GIT branch ${GIT_REFSPEC}")
#    message(STATUS "GIT revision ${GIT_SHA1}")
#    set (VCS_REVISION ${GIT_SHA1})
#endif()

find_package(sodium REQUIRED)

add_dependent_subproject(XDRPP)
add_dependent_subproject(SOCI)

set(VIXAL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

include_directories(
        ${sodium_INCLUDE_DIR}
        ${ASIO_INCLUDE_DIR}
        ${VIXAL_INCLUDE_DIR}
        ${PROJECT_BINARY_DIR}/include   # add the binary tree to the search path so that we will find Version.h
)

get_property(VIXAL_INCLUDE_DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

add_definitions(-DAUTO_INITIALIZE_EASYLOGGINGPP)
boost_report_value(VIXAL_INCLUDE_DIRS)
boost_report_value(CMAKE_CXX_FLAGS)
boost_report_value(LIB_SUFFIX)
boost_report_value(COMPILE_DEFINITIONS)

enable_testing()
add_subdirectory(include/xdr)
add_subdirectory(src)
# add_subdirectory(doc)
add_subdirectory(test)

#
# SYSTEM PACKAGING (RPM, TGZ, ...)
# _____________________________________________________________________________

include(CPackConfig)

#
# CMAKE PACKAGING (for other CMake projects to use this one easily)
# _____________________________________________________________________________

include(PackageConfigurator)
