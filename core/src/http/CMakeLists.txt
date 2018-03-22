set(Sources
        connection_manager.cpp
        HttpClient.cpp
        reply.cpp
        request_parser.cpp
        server.cpp
        request_handler.cpp
        connection.cpp
        )

set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/http/connection.hpp
        ${VIXAL_INCLUDE_DIR}/http/connection_manager.hpp
        ${VIXAL_INCLUDE_DIR}/http/header.hpp
        ${VIXAL_INCLUDE_DIR}/http/reply.hpp
        ${VIXAL_INCLUDE_DIR}/http/request.hpp
        ${VIXAL_INCLUDE_DIR}/http/request_parser.hpp
        ${VIXAL_INCLUDE_DIR}/http/server.hpp
        ${VIXAL_INCLUDE_DIR}/http/HttpClient.h
        ${VIXAL_INCLUDE_DIR}/http/request_handler.h)

add_library(http SHARED ${Sources} ${PublicHeaders})
target_link_libraries(http util)

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
