set(BUCKET_SOURCE_FILES
        ${XDR_COMPILED_FILES}
        ${VIXAL_INCLUDE_DIR}/bucket/Bucket.h
        ${VIXAL_INCLUDE_DIR}/bucket/BucketApplicator.h
        ${VIXAL_INCLUDE_DIR}/bucket/BucketList.h
        ${VIXAL_INCLUDE_DIR}/bucket/BucketManager.h
        ${VIXAL_INCLUDE_DIR}/bucket/FutureBucket.h
        ${VIXAL_INCLUDE_DIR}/bucket/LedgerCmp.h
        ${VIXAL_INCLUDE_DIR}/bucket/PublishQueueBuckets.h
        ${VIXAL_INCLUDE_DIR}/bucket/BucketInputIterator.h
        ${VIXAL_INCLUDE_DIR}/bucket/BucketOutputIterator.h
        Bucket.cpp
        BucketApplicator.cpp
        BucketList.cpp
        BucketManagerImpl.cpp
        FutureBucket.cpp
        PublishQueueBuckets.cpp
        BucketInputIterator.cpp
        BucketOutputIterator.cpp)

add_library(bucket SHARED ${BUCKET_SOURCE_FILES})
target_link_libraries(bucket crypto medida database ledger transactions)

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
