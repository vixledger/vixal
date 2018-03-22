set(Sources
        LoadGenerator.cpp
        Simulation.cpp
        Topologies.cpp
        )


set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/simulation/LoadGenerator.h
        ${VIXAL_INCLUDE_DIR}/simulation/Simulation.h
        ${VIXAL_INCLUDE_DIR}/simulation/Topologies.h
        )

add_library(simulation SHARED ${Sources} ${PublicHeaders})
target_link_libraries(simulation transactions ledger herder)


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
