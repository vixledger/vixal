set(Sources
        test.cpp
        TestAccount.cpp
        TestExceptions.cpp
        TestMarket.cpp
        TestPrinter.cpp
        TxTests.cpp
        TestUtils.cpp
        TxTests.cpp
        )


set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/test/test.h
        ${VIXAL_INCLUDE_DIR}/test/TestAccount.h
        ${VIXAL_INCLUDE_DIR}/test/TestExceptions.h
        ${VIXAL_INCLUDE_DIR}/test/TestPrinter.h
        ${VIXAL_INCLUDE_DIR}/test/TestUtils.h
        ${VIXAL_INCLUDE_DIR}/test/TxTests.h
        )

add_library(tests SHARED ${Sources} ${PublicHeaders})
target_link_libraries(tests application bucket)


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
