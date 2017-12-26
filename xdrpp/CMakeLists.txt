###############################################################################
# General settings
###############################################################################
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
###############################################################################
# XDRPP CMake modules
###############################################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake) # main (top) cmake dir

# Path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules) # project specific cmake dir

# CMake useful variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

project(XDRPP)

# Set the version number of your project here (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0.0)
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CPP_COMMAND "gcc -E -xc")
set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_VERSION "1.0")

include(Common)


###############################################################################
# Build features and variants
##############################################################################

option(XDRPP_SHARED "Enable build of shared libraries" ON)
option(XDRPP_STATIC "Enable build of static libraries" OFF)
option(XDRPP_TESTS "Enable build of collection of XDRPP tests" ON)


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

include(XdrppUtilities)
include(XdrppCompile)
include(TestBigEndian)

colormsg(_HIBLUE_ "Configuring XDRPP:")

TEST_BIG_ENDIAN(IS_BIG_ENDIAN)

###############################################################################
# XDRPP version information
###############################################################################
include(XdrppVersion)

xdrpp_version()

###############################################################################
# Build features and variants
##############################################################################

boost_report_value(XDRPP_SHARED)
boost_report_value(XDRPP_STATIC)
boost_report_value(XDRPP_TESTS)

# from XdrppCompile.cmake
boost_report_value(XDRPP_CXX_C11)
boost_report_value(LIB_SUFFIX)

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf
# directories. This simplifies manual testing and the use of the build
# tree rather than installed Boost libraries.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

###############################################################################
# Find XDRPP dependencies
###############################################################################

set(XDRPP_TARGET)
set(XDRPP_TARGET_STATIC)
set(XDRPP_DEPS_LIBS)

include(XdrppDependencies)

get_property(XDRPP_INCLUDE_DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  PROPERTY INCLUDE_DIRECTORIES)

if(NOT MSVC)
  set(DL_FIND_QUIETLY TRUE)
  find_package(DL)
  if(DL_FOUND)
    list(APPEND XDRPP_DEPS_LIBS ${DL_LIBRARY})
    set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES ${DL_INCLUDE_DIR})
    add_definitions(-DHAVE_DL=1)
  endif()
endif()

list(APPEND XDRPP_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  PROPERTY
  INCLUDE_DIRECTORIES ${XDRPP_INCLUDE_DIRS})

###############################################################################
# Installation
###############################################################################

if(NOT DEFINED XDRPP_LIBDIR)
  if(APPLE OR CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(XDRPP_LIBDIR "lib")
  else()
    set(XDRPP_LIBDIR "lib64")
  endif()
endif()

set(BINDIR "bin" CACHE PATH "The directory to install binaries into.")
set(LIBDIR ${XDRPP_LIBDIR} CACHE PATH "The directory to install libraries into.")
set(DATADIR "share" CACHE PATH "The directory to install data files into.")
set(INCLUDEDIR "include" CACHE PATH "The directory to install includes into.")

###############################################################################
# Configuration files
###############################################################################
set(CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
install(DIRECTORY ${CONFIG_INCLUDE_DIR}/xdrpp DESTINATION ${INCLUDEDIR})
set(CONFIG_FILE_IN "include/xdrpp/build_endian.h.in")
set(CONFIG_FILE_OUT "${CONFIG_INCLUDE_DIR}/xdrpp/build_endian.h")


include_directories(
        ${XDRPP_SOURCE_DIR}/include
        ${XDRPP_SOURCE_DIR}/src
        ${CONFIG_INCLUDE_DIR})
add_subdirectory(src)

if(XDRPP_TESTS)
  ###############################################################################
  # Enable tests
  ###############################################################################
  enable_testing()
  add_subdirectory(tests)
endif()

###############################################################################
# build config file
###############################################################################

get_cmake_property(ALL_VARIABLES CACHE_VARIABLES)
set(CONFIGURED_VARIABLES)
foreach(v ${ALL_VARIABLES})
    if (v MATCHES "^XDRPP_HAVE.*")
        get_property(CACHE_HELPSTRING CACHE ${v} PROPERTY HELPSTRING)
		set(CONFIGURED_VARIABLES "${CONFIGURED_VARIABLES}\n// ${CACHE_HELPSTRING}\n")
        if (${${v}})
            set(CONFIGURED_VARIABLES "${CONFIGURED_VARIABLES}#define ${v}\n")
        else()
            set(CONFIGURED_VARIABLES "${CONFIGURED_VARIABLES}/* #undef ${v} */\n")
        endif()
    endif()
endforeach()

configure_file("${CONFIG_FILE_IN}" "${CONFIG_FILE_OUT}")
configure_file(config.h.in ${CONFIG_INCLUDE_DIR}/xdrpp/config.h)

message(STATUS "")

#
# SYSTEM PACKAGING (RPM, TGZ, ...)
# _____________________________________________________________________________

include(CPackConfig)

#
# CMAKE PACKAGING (for other CMake projects to use this one easily)
# _____________________________________________________________________________

include(PackageConfigurator)

