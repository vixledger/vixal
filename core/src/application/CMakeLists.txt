configure_file(CoreVersion.cpp.in CoreVersion.cpp)

set(Sources
        NtpSynchronizationChecker.h
        Application.cpp
        ApplicationImpl.h
        ApplicationImpl.cpp
        ExternalQueue.cpp
        CommandHandler.cpp
        Config.cpp
        PersistentState.cpp
        NtpSynchronizationChecker.cpp
        Maintainer.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/CoreVersion.cpp
        )


set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/application/Application.h
        ${VIXAL_INCLUDE_DIR}/application/CommandHandler.h
        ${VIXAL_INCLUDE_DIR}/application/ExternalQueue.h
        ${VIXAL_INCLUDE_DIR}/application/PersistentState.h
        ${VIXAL_INCLUDE_DIR}/application/Config.h
        ${VIXAL_INCLUDE_DIR}/application/Maintainer.h
        )

add_library(application SHARED ${Sources} ${PublicHeaders})
target_link_libraries(application soci_core util medida jsoncpp http overlay catchup process simulation)


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
