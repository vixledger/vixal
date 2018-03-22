set(Sources
        OperationFrame.cpp
        AllowTrustOpFrame.cpp
        ChangeTrustOpFrame.cpp
        CreateAccountOpFrame.cpp
        CreatePassiveOfferOpFrame.cpp
        InflationOpFrame.cpp
        ManageDataOpFrame.cpp
        ManageOfferOpFrame.cpp
        MergeOpFrame.cpp
        OfferExchange.cpp
        PathPaymentOpFrame.cpp
        PaymentOpFrame.cpp
        SetOptionsOpFrame.cpp
        SignatureChecker.cpp
        SignerKeyUtils.cpp
        SignatureUtils.cpp
        TransactionFrame.cpp
        BumpSequenceOpFrame.cpp
        )

set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/transactions/OperationFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/AllowTrustOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/ChangeTrustOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/CreateAccountOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/CreatePassiveOfferOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/InflationOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/ManageDataOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/ManageOfferOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/MergeOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/OfferExchange.h
        ${VIXAL_INCLUDE_DIR}/transactions/PathPaymentOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/PaymentOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/SetOptionsOpFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/SignatureChecker.h
        ${VIXAL_INCLUDE_DIR}/transactions/SignatureUtils.h
        ${VIXAL_INCLUDE_DIR}/transactions/TransactionFrame.h
        ${VIXAL_INCLUDE_DIR}/transactions/BumpSequenceOpFrame.h
        )

add_library(transactions SHARED ${Sources} ${PublicHeaders})
target_link_libraries(transactions util ledger database)

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
