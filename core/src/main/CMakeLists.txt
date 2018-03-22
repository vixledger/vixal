set(Sources
        dumpxdr.cpp
        main.cpp
        )

set(CATCH2_SOURCE_FILES
        ${VIXAL_INCLUDE_DIR}/catch.hpp
        ${VIXAL_INCLUDE_DIR}/clara.hpp)

set(AUTOCHECK_HEADERS
        ${VIXAL_INCLUDE_DIR}/autocheck/apply.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/arbitrary.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/autocheck.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/check.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/classifier.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/distribution.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/function.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/generator.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/generator_combinators.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/is_one_of.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/largest.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/ostream.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/reporter.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/sequence.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/tuple.hpp
        ${VIXAL_INCLUDE_DIR}/autocheck/value.hpp
        )

set(CEREAL_HEADERS
        ${VIXAL_INCLUDE_DIR}/cereal/cereal.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/macros.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/access.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/base_class.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/unordered_map.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/boost_variant.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/utility.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/bitset.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/array.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/polymorphic.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/vector.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/map.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/complex.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/tuple.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/queue.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/list.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/forward_list.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/functional.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/set.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/valarray.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/string.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/chrono.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/deque.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/stack.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/memory.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/common.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/unordered_set.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/types/concepts/pair_associative_container.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/details/static_object.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/details/polymorphic_impl_fwd.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/details/traits.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/details/polymorphic_impl.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/details/helpers.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/details/util.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/archives/json.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/archives/adapters.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/archives/xml.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/archives/portable_binary.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/archives/binary.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/fwd.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/memorystream.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/reader.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/prettywriter.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/ostreamwrapper.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/encodedstream.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/filereadstream.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/istreamwrapper.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/stringbuffer.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/ieee754.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/strtod.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/swap.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/regex.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/diyfp.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/biginteger.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/strfunc.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/itoa.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/stack.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/dtoa.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/meta.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/internal/pow10.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/encodings.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/schema.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/stream.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/filewritestream.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/rapidjson.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/document.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/allocators.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/writer.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/error/error.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/error/en.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/memorybuffer.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/pointer.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/msinttypes/inttypes.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidjson/msinttypes/stdint.h
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidxml/rapidxml_iterators.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidxml/rapidxml_print.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidxml/rapidxml_utils.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidxml/rapidxml.hpp
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidxml/license.txt
        ${VIXAL_INCLUDE_DIR}/cereal/external/rapidxml/manual.html
        ${VIXAL_INCLUDE_DIR}/cereal/external/base64.hpp
        )

set(PublicHeaders
        ${VIXAL_INCLUDE_DIR}/catch.hpp
        ${VIXAL_INCLUDE_DIR}/xdr/xdr.h
        ${VIXAL_INCLUDE_DIR}/CoreVersion.h
        ${CEREAL_HEADERS}
        ${AUTOCHECK_HEADERS}
        ${XDR_COMPILED_FILES}
        )

set(APPLICATION_NAME vixal-core)

add_executable(${APPLICATION_NAME}
        ${Sources} ${PublicHeaders}
        )

target_link_libraries(${APPLICATION_NAME}
        bucket ledger overlay scp work process
        database invariant transactions herder history
        historywork catchup
        application simulation tests
        sodium
        soci_core soci_sqlite3
        soci_postgresql
        xdrpp)

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
