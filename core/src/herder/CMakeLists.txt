set(Sources
        Herder.cpp
        HerderImpl.cpp
        HerderPersistenceImpl.cpp
        HerderSCPDriver.cpp
        HerderUtils.cpp
        LedgerCloseData.cpp
        PendingEnvelopes.cpp
        TxSetFrame.cpp
        Upgrades.cpp
        )

set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/herder/Herder.h
        ${VIXAL_INCLUDE_DIR}/herder/HerderPersistence.h
        ${VIXAL_INCLUDE_DIR}/herder/HerderSCPDriver.h
        ${VIXAL_INCLUDE_DIR}/herder/HerderUtils.h
        ${VIXAL_INCLUDE_DIR}/herder/TxSetFrame.h
        ${VIXAL_INCLUDE_DIR}/herder/LedgerCloseData.h
        ${VIXAL_INCLUDE_DIR}/herder/Upgrades.h
        )

add_library(herder SHARED ${Sources} ${PublicHeaders})
target_link_libraries(herder scp)


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
