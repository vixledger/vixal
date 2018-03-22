set(Sources
        BatchDownloadWork.cpp
        BucketDownloadWork.cpp
        FetchRecentQsetsWork.cpp
        GetAndUnzipRemoteFileWork.cpp
        GetHistoryArchiveStateWork.cpp
        GetRemoteFileWork.cpp
        GunzipFileWork.cpp
        GzipFileWork.cpp
        MakeRemoteDirWork.cpp
        Progress.cpp
        PublishWork.cpp
        PutHistoryArchiveStateWork.cpp
        PutRemoteFileWork.cpp
        PutSnapshotFilesWork.cpp
        RepairMissingBucketsWork.cpp
        ResolveSnapshotWork.cpp
        RunCommandWork.cpp
        VerifyBucketWork.cpp
        WriteSnapshotWork.cpp
        )


set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/historywork/BatchDownloadWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/BucketDownloadWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/FetchRecentQsetsWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/GetAndUnzipRemoteFileWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/GetHistoryArchiveStateWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/GetRemoteFileWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/GunzipFileWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/GzipFileWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/MakeRemoteDirWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/Progress.h
        ${VIXAL_INCLUDE_DIR}/historywork/PublishWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/PutHistoryArchiveStateWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/PutRemoteFileWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/RepairMissingBucketsWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/ResolveSnapshotWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/PutSnapshotFilesWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/RunCommandWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/VerifyBucketWork.h
        ${VIXAL_INCLUDE_DIR}/historywork/WriteSnapshotWork.h
        )

add_library(historywork SHARED ${Sources} ${PublicHeaders})


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
