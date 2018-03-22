set(Sources
        CacheIsConsistentWithDatabase.cpp
        InvariantDoesNotHold.cpp
        InvariantManagerImpl.cpp
        BucketListIsConsistentWithDatabase.cpp
        ConservationOfLumens.cpp
        MinimumAccountBalance.cpp
        InvariantTestUtils.cpp
        AccountSubEntriesCountIsValid.cpp
        LedgerEntryIsValid.cpp
        )

set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/invariant/Invariant.h
        ${VIXAL_INCLUDE_DIR}/invariant/CacheIsConsistentWithDatabase.h
        ${VIXAL_INCLUDE_DIR}/invariant/InvariantDoesNotHold.h
        ${VIXAL_INCLUDE_DIR}/invariant/InvariantManager.h
        ${VIXAL_INCLUDE_DIR}/invariant/BucketListIsConsistentWithDatabase.h
        ${VIXAL_INCLUDE_DIR}/invariant/ConservationOfLumens.h
        ${VIXAL_INCLUDE_DIR}/invariant/MinimumAccountBalance.h
        ${VIXAL_INCLUDE_DIR}/invariant/LedgerEntryIsValid.h
        ${VIXAL_INCLUDE_DIR}/invariant/InvariantTestUtils.h
        ${VIXAL_INCLUDE_DIR}/invariant/AccountSubEntriesCountIsValid.h
        )

add_library(invariant SHARED ${Sources} ${PublicHeaders})


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
