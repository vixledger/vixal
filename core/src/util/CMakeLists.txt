set(Sources
        BitsetEnumerator.cpp
        Fs.cpp
        GlobalChecks.cpp
        Logging.cpp
        Math.cpp
        NtpClient.cpp
        SecretValue.cpp
        StatusManager.cpp
        Timer.cpp
        TmpDir.cpp
        types.cpp
        easylogging++.cpp
        format.cc
        getopt_long.c
        uint128_t.cpp
        MetricResetter.cpp)


set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/util/Algoritm.h
        ${VIXAL_INCLUDE_DIR}/util/asio.h
        ${VIXAL_INCLUDE_DIR}/util/basen.h
        ${VIXAL_INCLUDE_DIR}/util/BitsetEnumerator.h
        ${VIXAL_INCLUDE_DIR}/util/cpptoml.h
        ${VIXAL_INCLUDE_DIR}/crypto/crc16.h
        ${VIXAL_INCLUDE_DIR}/util/easylogging++.h
        ${VIXAL_INCLUDE_DIR}/util/format.h
        ${VIXAL_INCLUDE_DIR}/util/Fs.h
        ${VIXAL_INCLUDE_DIR}/util/getopt.h
        ${VIXAL_INCLUDE_DIR}/util/GlobalChecks.h
        ${VIXAL_INCLUDE_DIR}/util/Logging.h
        ${VIXAL_INCLUDE_DIR}/util/lrucache.hpp
        ${VIXAL_INCLUDE_DIR}/util/Math.h
        ${VIXAL_INCLUDE_DIR}/util/must_use.h
        ${VIXAL_INCLUDE_DIR}/util/noncopyable.h
        ${VIXAL_INCLUDE_DIR}/util/NtpClient.h
        ${VIXAL_INCLUDE_DIR}/work/NtpWork.h
        ${VIXAL_INCLUDE_DIR}/util/optional.hpp
        ${VIXAL_INCLUDE_DIR}/util/SecretValue.h
        ${VIXAL_INCLUDE_DIR}/util/StatusManager.h
        ${VIXAL_INCLUDE_DIR}/util/Timer.h
        ${VIXAL_INCLUDE_DIR}/util/TmpDir.h
        ${VIXAL_INCLUDE_DIR}/util/types.h
        ${VIXAL_INCLUDE_DIR}/util/uint128_t.h
        ${VIXAL_INCLUDE_DIR}/util/XDRStream.h
        ${VIXAL_INCLUDE_DIR}/util/tasks_processor_base.hpp
        ${VIXAL_INCLUDE_DIR}/util/abstract_clock.hpp
        ${VIXAL_INCLUDE_DIR}/util/manual_clock.hpp
        ${VIXAL_INCLUDE_DIR}/util/SimpleTestReporter.hpp
        ${VIXAL_INCLUDE_DIR}/util/MetricResetter.h)

add_library(util SHARED ${Sources} ${PublicHeaders})
target_link_libraries(util crypto)

add_library(asio SHARED asio.cpp)

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
