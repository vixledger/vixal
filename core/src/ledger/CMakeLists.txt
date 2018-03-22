set(Sources
        AccountFrame.cpp
        CheckpointRange.cpp
        DataFrame.cpp
        EntryFrame.cpp
        LedgerDelta.cpp
        LedgerHeaderFrame.cpp
        LedgerManagerImpl.cpp
        LedgerRange.cpp
        OfferFrame.cpp
        SyncingLedgerChain.cpp
        TrustFrame.cpp
        LedgerTestUtils.cpp
        )

set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/ledger/AccountFrame.h
        ${VIXAL_INCLUDE_DIR}/ledger/DataFrame.h
        ${VIXAL_INCLUDE_DIR}/ledger/EntryFrame.h
        ${VIXAL_INCLUDE_DIR}/ledger/OfferFrame.h
        ${VIXAL_INCLUDE_DIR}/ledger/TrustFrame.h
        ${VIXAL_INCLUDE_DIR}/ledger/LedgerHeaderFrame.h
        ${VIXAL_INCLUDE_DIR}/ledger/LedgerDelta.h
        ${VIXAL_INCLUDE_DIR}/ledger/LedgerRange.h
        ${VIXAL_INCLUDE_DIR}/ledger/SyncingLedgerChain.h
        ${VIXAL_INCLUDE_DIR}/ledger/CheckpointRange.h
        ${VIXAL_INCLUDE_DIR}/ledger/LedgerManager.h
        ${VIXAL_INCLUDE_DIR}/ledger/LedgerTestUtils.h
        )

add_library(ledger SHARED ${Sources} ${PublicHeaders})
target_link_libraries(ledger herder invariant)


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
