set(Sources
        ApplyBucketsWork.cpp
        ApplyLedgerChainWork.cpp
        CatchupConfiguration.cpp
        CatchupManagerImpl.cpp
        CatchupWork.cpp
        DownloadBucketsWork.cpp
        VerifyLedgerChainWork.cpp
        )

set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/catchup/CatchupConfiguration.h
        ${VIXAL_INCLUDE_DIR}/catchup/CatchupWork.h
        ${VIXAL_INCLUDE_DIR}/catchup/CatchupManager.h
        ${VIXAL_INCLUDE_DIR}/catchup/ApplyBucketsWork.h
        ${VIXAL_INCLUDE_DIR}/catchup/ApplyLedgerChainWork.h
        ${VIXAL_INCLUDE_DIR}/catchup/DownloadBucketsWork.h
        ${VIXAL_INCLUDE_DIR}/catchup/VerifyLedgerChainWork.h
        )

add_library(catchup SHARED ${Sources} ${PublicHeaders})
target_link_libraries(catchup ledger history historywork)


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
