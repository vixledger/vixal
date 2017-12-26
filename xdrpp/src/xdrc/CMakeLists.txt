FIND_PACKAGE(BISON REQUIRED)
SET(BisonOutput ${XDRPP_SOURCE_DIR}/src/xdrc/parse.cc)
IF (BISON_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${BisonOutput}
            COMMAND ${BISON_EXECUTABLE}
            --defines=${XDRPP_SOURCE_DIR}/src/xdrc/tokens.h
            --output=${BisonOutput}
            ${XDRPP_SOURCE_DIR}/src/xdrc/parse.yy
            COMMENT "Generating parse.cc"
    )
ENDIF ()

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput ${XDRPP_SOURCE_DIR}/src/xdrc/scan.cc)
IF (FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${FlexOutput}
            COMMAND ${FLEX_EXECUTABLE}
            --outfile=${FlexOutput}
            ${XDRPP_SOURCE_DIR}/src/xdrc/scan.ll
            COMMENT "Generating scan.cc"
    )
ENDIF ()


set(APP_HEADERS
        union.h
        xdrc_internal.h
        )

set(APP_SOURCES
        xdrc.cc
        gen_hh.cc
        gen_server.cc
        )

# Set the build version. It will be used in the name of the lib, with corresponding
# symlinks created. SOVERSION could also be specified for api version. 
set_target_properties(${LIBRARY_NAME} PROPERTIES
        VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
        FRAMEWORK FALSE
        PUBLIC_HEADER "${LIB_HEADERS}" # specify the public headers
        )

# This is a make target, so you can do a "make xdrc"
set(APPLICATION_NAME xdrc)
add_executable(${APPLICATION_NAME}
        ${APP_SOURCES}
        ${APP_HEADERS}
        ${BisonOutput}
        ${FlexOutput}
        )

# Links the target exe (xdrc) against the library (libprojA) and other required libraries
target_link_libraries(${APPLICATION_NAME} ${LIBRARY_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Says how and where to install software
# Targets:
#   * <prefix>/lib/<libraries>
#   * header location after install: <prefix>/include/<project>/*.h
#   * headers can be included by C++ code `#<project>/Bar.hpp>`
install(TARGETS ${LIBRARY_NAME} ${APPLICATION_NAME}
        EXPORT ${PROJECT_NAME}Targets            # for downstream dependencies
        ARCHIVE DESTINATION lib COMPONENT libs   # static lib
        RUNTIME DESTINATION bin COMPONENT libs   # binaries
        LIBRARY DESTINATION lib COMPONENT libs   # shared lib
        FRAMEWORK DESTINATION bin COMPONENT libs # for mac
        PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} COMPONENT devel   # headers for mac (note the different component -> different package)
        INCLUDES DESTINATION include             # headers
        )

