colormsg(_HIBLUE_ "Configuring XDRPP library:")

# Set INCLUDE_DIRECTORIES
get_directory_property(XDRPP_INCLUDE_DIRS INCLUDE_DIRECTORIES)
list(APPEND XDRPP_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})
set_directory_properties(PROPERTIES
        INCLUDE_DIRECTORIES "${XDRPP_INCLUDE_DIRS}")

# Core source files
file(GLOB XDRPP_HEADERS ${XDRPP_SOURCE_DIR}/include/xpdr/*.h)
set(XDRPP_SOURCES
        arpc.cc
        iniparse.cc
        marshal.cc
        msgsock.cc
        pollset.cc
        printer.cc
        rpc_msg.cc
        rpcbind.cc
        server.cc
        socket.cc
        socket_unix.cc
        srpc.cc
        )

# Group source files for IDE source explorers (e.g. Visual Studio)
source_group("Header Files" FILES ${XDRPP_HEADERS})
source_group("Source Files" FILES ${XDRPP_SOURCES})
source_group("CMake Files" FILES CMakeLists.txt)

# Core targets configuration
string(TOLOWER "${PROJECT_NAME}" PROJECTNAMEL)
#this command will update parent scope variable
set(XDRPP_TARGET ${PROJECTNAMEL} PARENT_SCOPE)
set(XDRPP_TARGET ${PROJECTNAMEL})

xdrpp_target_output_name(${XDRPP_TARGET} XDRPP_TARGET_OUTPUT_NAME)

#
# XDRPP shared library
#
if (XDRPP_SHARED)
    add_library(${XDRPP_TARGET} SHARED ${XDRPP_HEADERS} ${XDRPP_SOURCES})

    target_link_libraries(${XDRPP_TARGET} ${XDRPP_DEPS_LIBS})

    if(WIN32)
        set_target_properties(${XDRPP_TARGET}
                PROPERTIES
                DEFINE_SYMBOL XDRPP_DLL
                OUTPUT_NAME "${XDRPP_TARGET_OUTPUT_NAME}"
                VERSION ${XDRPP_VERSION}
                CLEAN_DIRECT_OUTPUT 1)
    else()
        set_target_properties(${XDRPP_TARGET}
                PROPERTIES
                VERSION ${XDRPP_VERSION}
                SOVERSION ${XDRPP_SOVERSION}
                INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib
                CLEAN_DIRECT_OUTPUT 1)
    endif()

    target_include_directories(${XDRPP_TARGET}
            PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
            )

endif()

# This adds definitions to all build configurations. XDRPP_DEBUG_POSTFIX is passed to xdrpp library
add_definitions(-DXDRPP_LIB_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}xdrpp_"
        -DXDRPP_LIB_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}"
        -DXDRPP_DEBUG_POSTFIX="${CMAKE_DEBUG_POSTFIX}")

#
# Core static library
#
if (XDRPP_STATIC)
    set(XDRPP_TARGET_STATIC ${XDRPP_TARGET}_static)

    add_library(${XDRPP_TARGET_STATIC} STATIC
            ${XDRPP_HEADERS} ${XDRPP_SOURCES})

    # we still need to link against dl if we have it
    target_link_libraries (${XDRPP_TARGET_STATIC}
            ${XDRPP_DEPS_LIBS}
            )

    set_target_properties(${XDRPP_TARGET_STATIC}
            PROPERTIES
            OUTPUT_NAME ${XDRPP_TARGET_OUTPUT_NAME}
            PREFIX "lib"
            CLEAN_DIRECT_OUTPUT 1)

    target_include_directories(${XDRPP_TARGET_STATIC}
            PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
            )

endif()

#
# Core installation
#
install(FILES ${XDRPP_HEADERS} DESTINATION ${INCLUDEDIR}/${PROJECTNAMEL})

if (XDRPP_SHARED)
    install(TARGETS ${XDRPP_TARGET}
            EXPORT XDRPPTargets
            RUNTIME DESTINATION ${BINDIR}
            LIBRARY DESTINATION ${LIBDIR}
            ARCHIVE DESTINATION ${LIBDIR})
endif()

if (XDRPP_STATIC)
    install(TARGETS ${XDRPP_TARGET_STATIC}
            EXPORT XDRPPTargets
            RUNTIME DESTINATION ${BINDIR}
            LIBRARY DESTINATION ${LIBDIR}
            ARCHIVE DESTINATION ${LIBDIR})
endif()

install(EXPORT XDRPPTargets NAMESPACE XDRPP:: DESTINATION cmake)

#
# Core configuration summary
#
boost_report_value(XDRPP_TARGET)
boost_report_value(XDRPP_TARGET_OUTPUT_NAME)
boost_report_value(XDRPP_DEPS_LIBS)
boost_report_value(XDRPP_INCLUDE_DIRS)
boost_report_value(CMAKE_CXX_FLAGS)
xdrpp_report_directory_property(COMPILE_DEFINITIONS)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}/include   # add the binary tree to the search path so that we will find Version.h
)


message(STATUS "")
