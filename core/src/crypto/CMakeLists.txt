set(CRYPTO_SOURCE_FILES
        ${VIXAL_INCLUDE_DIR}/crypto/ByteSlice.h
        ${VIXAL_INCLUDE_DIR}/crypto/ECDH.h
        ${VIXAL_INCLUDE_DIR}/crypto/Hex.h
        ${VIXAL_INCLUDE_DIR}/crypto/KeyUtils.h
        ${VIXAL_INCLUDE_DIR}/crypto/Random.h
        ${VIXAL_INCLUDE_DIR}/crypto/SecretKey.h
        ${VIXAL_INCLUDE_DIR}/crypto/SignerKey.h
        ${VIXAL_INCLUDE_DIR}/crypto/StrKey.h
        ${VIXAL_INCLUDE_DIR}/crypto/SHA.h
        ${VIXAL_INCLUDE_DIR}/crypto/crc16.h
        ${VIXAL_INCLUDE_DIR}/crypto/HashOfHash.h
        crc16.cpp
        ECDH.cpp
        Hex.cpp
        KeyUtils.cpp
        Random.cpp
        SHA.cpp
        SecretKey.cpp
        SignerKey.cpp
        HashOfHash.cpp
        StrKey.cpp)
add_library(crypto SHARED ${CRYPTO_SOURCE_FILES})

target_link_libraries(crypto sodium asio)

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
