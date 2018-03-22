set(Sources
        Floodgate.cpp
        ItemFetcher.cpp
        LoadManager.cpp
        LoopbackPeer.cpp
        Peer.cpp
        PeerAuth.cpp
        PeerDoor.cpp
        PeerRecord.cpp
        TCPPeer.cpp
        Tracker.cpp
        OverlayManagerImpl.cpp
        BanManagerImpl.cpp
        PeerBareAddress.cpp
        PeerSharedKeyId.cpp
        )


set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/overlay/Peer.h
        ${VIXAL_INCLUDE_DIR}/overlay/TCPPeer.h
        ${VIXAL_INCLUDE_DIR}/overlay/PeerAuth.h
        ${VIXAL_INCLUDE_DIR}/overlay/PeerDoor.h
        ${VIXAL_INCLUDE_DIR}/overlay/PeerRecord.h
        ${VIXAL_INCLUDE_DIR}/overlay/LoopbackPeer.h
        ${VIXAL_INCLUDE_DIR}/overlay/OverlayManager.h
        ${VIXAL_INCLUDE_DIR}/overlay/BanManager.h
        ${VIXAL_INCLUDE_DIR}/overlay/Tracker.h
        ${VIXAL_INCLUDE_DIR}/overlay/Floodgate.h
        ${VIXAL_INCLUDE_DIR}/overlay/ItemFetcher.h
        ${VIXAL_INCLUDE_DIR}/overlay/LoadManager.h
        ${VIXAL_INCLUDE_DIR}/overlay/PeerBareAddress.h
        ${VIXAL_INCLUDE_DIR}/overlay/PeerSharedKeyId.h
        )

add_library(overlay SHARED ${Sources} ${PublicHeaders})
target_link_libraries(overlay util medida herder)


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
