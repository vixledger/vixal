set(Sources
        BallotProtocol.cpp
        LocalNode.cpp
        NominationProtocol.cpp
        QuorumSetUtils.cpp
        scp.cpp
        SCPDriver.cpp
        Slot.cpp
        )


set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/scp/SCP.h
        ${VIXAL_INCLUDE_DIR}/scp/SCPDriver.h
        ${VIXAL_INCLUDE_DIR}/scp/LocalNode.h
        ${VIXAL_INCLUDE_DIR}/scp/Slot.h
        ${VIXAL_INCLUDE_DIR}/scp/QuorumSetUtils.h
        ${VIXAL_INCLUDE_DIR}/scp/NominationProtocol.h
        ${VIXAL_INCLUDE_DIR}/scp/BallotProtocol.h
        )

add_library(scp SHARED ${Sources} ${PublicHeaders})
target_link_libraries(scp xdrpp jsoncpp)


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
