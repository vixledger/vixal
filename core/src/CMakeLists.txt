set(VIXAL_CORE_HEADER_FILES
        ../include/xdr/ledger-entries.h
        ../include/xdr/ledger.h
        ../include/xdr/overlay.h
        ../include/xdr/scp.h
        ../include/xdr/types.h
        ../include/xdr/transaction.h
        ../include/xdr/xdr.h
        )

set(CATCH2_SOURCE_FILES
        ../include/catch.hpp
        ../include/clara.hpp)

set(MEDIDA_SOURCE_FILES
        ../include/medida/types.h
        ../include/medida/timer_context.h
        ../include/medida/timer.h
        ../include/medida/counter.h
        ../include/medida/histogram.h
        ../include/medida/medida.h
        ../include/medida/meter.h
        ../include/medida/metered_interface.h
        ../include/medida/metric_interface.h
        ../include/medida/metric_processor.h
        ../include/medida/metrics_registry.h
        ../include/medida/metric_name.h
        ../include/medida/sampling_interface.h
        ../include/medida/summarizable_interface.h
        ../include/medida/reporting/abstract_polling_reporter.h
        ../include/medida/reporting/collectd_reporter.h
        ../include/medida/reporting/collectd_reporter.h
        ../include/medida/reporting/console_reporter.h
        ../include/medida/reporting/json_reporter.h
        ../include/medida/reporting/util.h
        ../include/medida/stats/ewma.h
        ../include/medida/stats/snapshot.h
        ../include/medida/stats/sample.h
        ../include/medida/stats/uniform_sample.h
        ../include/medida/stats/exp_decay_sample.h
        medida/reporting/console_reporter.cc
        medida/reporting/json_reporter.cc
        medida/reporting/util.cc
        medida/reporting/abstract_polling_reporter.cc
        medida/reporting/collectd_reporter.cc
        medida/timer_context.cc
        medida/metric_processor.cc
        medida/counter.cc
        medida/metric_name.cc
        medida/histogram.cc
        medida/timer.cc
        medida/metrics_registry.cc
        medida/meter.cc
        medida/stats/snapshot.cc
        medida/stats/ewma.cc
        medida/stats/exp_decay_sample.cc
        medida/stats/uniform_sample.cc
        )

add_library(medida SHARED ${MEDIDA_SOURCE_FILES})

set(HTTP_SOURCE_FILES
        ../include/http/connection.hpp
        ../include/http/connection_manager.hpp
        ../include/http/header.hpp
        ../include/http/reply.hpp
        ../include/http/request.hpp
        ../include/http/request_parser.hpp
        ../include/http/server.hpp
        ../include/http/HttpClient.h
        http/connection.cpp
        http/connection_manager.cpp
        http/HttpClient.cpp
        http/reply.cpp
        http/request_parser.cpp
        http/server.cpp
        )

add_library(http SHARED ${HTTP_SOURCE_FILES})
target_link_libraries(http util)

set(CRYPTO_SOURCE_FILES
        ../include/crypto/ByteSlice.h
        ../include/crypto/ECDH.h
        ../include/crypto/Hex.h
        ../include/crypto/KeyUtils.h
        ../include/crypto/Random.h
        ../include/crypto/SecretKey.h
        ../include/crypto/SignerKey.h
        ../include/transactions/SignerKeyUtils.h
        ../include/crypto/StrKey.h
        ../include/crypto/SHA.h
        ../include/crypto/crc16.h
        ../include/crypto/HashOfHash.h
        crypto/crc16.cpp
        crypto/ECDH.cpp
        crypto/Hex.cpp
        crypto/KeyUtils.cpp
        crypto/Random.cpp
        crypto/SHA.cpp
        crypto/SecretKey.cpp
        crypto/SignerKey.cpp
        crypto/HashOfHash.cpp
        crypto/StrKey.cpp)
add_library(crypto SHARED ${CRYPTO_SOURCE_FILES})

target_link_libraries(crypto sodium asio)

add_library(asio SHARED util/asio.cpp)

set(JSONCPP_SOURCE_FILES
        ../include/json/json-forwards.h
        ../include/json/json.h
        json/jsoncpp.cpp
        )
add_library(jsoncpp SHARED ${JSONCPP_SOURCE_FILES})

set(UTIL_SOURCE_FILES
        ../include/util/Algoritm.h
        ../include/util/asio.h
        ../include/util/basen.h
        ../include/util/BitsetEnumerator.h
        ../include/util/cpptoml.h
        ../include/crypto/crc16.h
        ../include/util/easylogging++.h
        ../include/util/format.h
        ../include/util/Fs.h
        ../include/util/getopt.h
        ../include/util/GlobalChecks.h
        ../include/util/Logging.h
        ../include/util/lrucache.hpp
        ../include/util/Math.h
        ../include/util/must_use.h
        ../include/util/noncopyable.h
        ../include/util/NtpClient.h
        ../include/work/NtpWork.h
        ../include/util/optional.hpp
        ../include/util/SecretValue.h
        ../include/util/SociNoWarnings.h
        ../include/util/StatusManager.h
        ../include/util/StatusManager.h
        ../include/util/Timer.h
        ../include/util/TmpDir.h
        ../include/util/types.h
        ../include/util/uint128_t.h
        ../include/util/XDRStream.h

        ../include/util/SimpleTestReporter.hpp
        util/BitsetEnumerator.cpp
        util/Fs.cpp
        util/GlobalChecks.cpp
        util/Logging.cpp
        util/Math.cpp
        util/NtpClient.cpp
        util/SecretValue.cpp
        util/StatusManager.cpp
        util/Timer.cpp
        util/TmpDir.cpp
        util/types.cpp
        util/easylogging++.cpp
        util/format.cc
        util/getopt_long.c
        util/uint128_t.cpp
        ../include/util/tasks_processor_base.hpp
        ../include/util/abstract_clock.hpp
        ../include/util/manual_clock.hpp)

add_library(util SHARED ${UTIL_SOURCE_FILES})
target_link_libraries(util crypto)

set(BUCKET_SOURCE_FILES
        ${XDR_COMPILED_FILES}
        ../include/bucket/Bucket.h
        ../include/bucket/BucketApplicator.h
        ../include/bucket/BucketList.h
        ../include/bucket/BucketManager.h
        ../include/bucket/FutureBucket.h
        ../include/bucket/LedgerCmp.h
        ../include/bucket/PublishQueueBuckets.h
        bucket/Bucket.cpp
        bucket/BucketApplicator.cpp
        bucket/BucketList.cpp
        bucket/BucketManagerImpl.cpp
        bucket/FutureBucket.cpp
        bucket/PublishQueueBuckets.cpp
        ../include/bucket/BucketInputIterator.h bucket/BucketInputIterator.cpp ../include/bucket/BucketOutputIterator.h bucket/BucketOutputIterator.cpp)

add_library(bucket SHARED ${BUCKET_SOURCE_FILES})
target_link_libraries(bucket crypto medida database ledger transactions)

set(LEDGER_SOURCE_FILES
        ../include/ledger/AccountFrame.h
        ../include/ledger/DataFrame.h
        ../include/ledger/EntryFrame.h
        ../include/ledger/OfferFrame.h
        ../include/ledger/TrustFrame.h
        ../include/ledger/LedgerHeaderFrame.h
        ../include/ledger/LedgerDelta.h
        ../include/ledger/LedgerRange.h
        ../include/ledger/SyncingLedgerChain.h
        ../include/ledger/CheckpointRange.h
        ../include/ledger/LedgerManager.h
        ../include/ledger/LedgerTestUtils.h

        ledger/AccountFrame.cpp
        ledger/CheckpointRange.cpp
        ledger/DataFrame.cpp
        ledger/EntryFrame.cpp
        ledger/LedgerDelta.cpp
        ledger/LedgerHeaderFrame.cpp
        ledger/LedgerManagerImpl.cpp
        ledger/LedgerRange.cpp
        ledger/OfferFrame.cpp
        ledger/SyncingLedgerChain.cpp
        ledger/TrustFrame.cpp
        ledger/LedgerTestUtils.cpp
        )

add_library(ledger SHARED ${LEDGER_SOURCE_FILES})
target_link_libraries(ledger herder invariant)

set(OVERLAY_SOURCE_FILES
        ../include/overlay/Peer.h
        ../include/overlay/TCPPeer.h
        ../include/overlay/PeerAuth.h
        ../include/overlay/PeerDoor.h
        ../include/overlay/PeerRecord.h
        ../include/overlay/LoopbackPeer.h
        ../include/overlay/OverlayManager.h
        ../include/overlay/BanManager.h
        ../include/overlay/Tracker.h
        ../include/overlay/Floodgate.h
        ../include/overlay/ItemFetcher.h
        ../include/overlay/LoadManager.h

        overlay/Floodgate.cpp
        overlay/ItemFetcher.cpp
        overlay/LoadManager.cpp
        overlay/LoopbackPeer.cpp
        overlay/Peer.cpp
        overlay/PeerAuth.cpp
        overlay/PeerDoor.cpp
        overlay/PeerRecord.cpp
        overlay/TCPPeer.cpp
        overlay/Tracker.cpp
        overlay/OverlayManagerImpl.cpp
        overlay/BanManagerImpl.cpp
        )

add_library(overlay SHARED ${OVERLAY_SOURCE_FILES})
target_link_libraries(overlay util medida herder)

set(SCP_SOURCE_FILES
        ../include/scp/SCP.h
        ../include/scp/SCPDriver.h
        ../include/scp/LocalNode.h
        ../include/scp/Slot.h
        ../include/scp/QuorumSetUtils.h
        ../include/scp/NominationProtocol.h
        ../include/scp/BallotProtocol.h
        scp/BallotProtocol.cpp
        scp/LocalNode.cpp
        scp/NominationProtocol.cpp
        scp/QuorumSetUtils.cpp
        scp/scp.cpp
        scp/SCPDriver.cpp
        scp/Slot.cpp
        )
add_library(scp SHARED ${SCP_SOURCE_FILES})
target_link_libraries(scp xdrpp jsoncpp)

set(DATABASE_SOURCE_FILES
        ../include/database/Database.h
        database/Database.cpp
        database/DatabaseConnectionString.cpp
        )

add_library(database SHARED ${DATABASE_SOURCE_FILES})
target_link_libraries(database util medida soci_core soci_sqlite3 soci_postgresql)

set(TRANSACTIONS_SOURCE_FILES
        ../include/transactions/OperationFrame.h
        ../include/transactions/AllowTrustOpFrame.h
        ../include/transactions/ChangeTrustOpFrame.h
        ../include/transactions/CreateAccountOpFrame.h
        ../include/transactions/CreatePassiveOfferOpFrame.h
        ../include/transactions/InflationOpFrame.h
        ../include/transactions/ManageDataOpFrame.h
        ../include/transactions/ManageOfferOpFrame.h
        ../include/transactions/MergeOpFrame.h
        ../include/transactions/OfferExchange.h
        ../include/transactions/PathPaymentOpFrame.h
        ../include/transactions/PaymentOpFrame.h
        ../include/transactions/SetOptionsOpFrame.h
        ../include/transactions/SignatureChecker.h
        ../include/transactions/SignatureUtils.h
        ../include/transactions/TransactionFrame.h

        transactions/OperationFrame.cpp
        transactions/AllowTrustOpFrame.cpp
        transactions/ChangeTrustOpFrame.cpp
        transactions/CreateAccountOpFrame.cpp
        transactions/CreatePassiveOfferOpFrame.cpp
        transactions/InflationOpFrame.cpp
        transactions/ManageDataOpFrame.cpp
        transactions/ManageOfferOpFrame.cpp
        transactions/MergeOpFrame.cpp
        transactions/OfferExchange.cpp
        transactions/PathPaymentOpFrame.cpp
        transactions/PaymentOpFrame.cpp
        transactions/SetOptionsOpFrame.cpp
        transactions/SignatureChecker.cpp
        transactions/SignerKeyUtils.cpp
        transactions/SignatureUtils.cpp
        transactions/TransactionFrame.cpp
        )

add_library(transactions SHARED ${TRANSACTIONS_SOURCE_FILES})
target_link_libraries(transactions util ledger database)

set(INVARIANT_SOURCE_FILES
        ../include/invariant/Invariant.h
        ../include/invariant/CacheIsConsistentWithDatabase.h
        ../include/invariant/InvariantDoesNotHold.h
        ../include/invariant/InvariantManager.h
        ../include/invariant/BucketListIsConsistentWithDatabase.h
        ../include/invariant/ConservationOfLumens.h
        ../include/invariant/MinimumAccountBalance.h
        ../include/invariant/LedgerEntryIsValid.h
        ../include/invariant/InvariantTestUtils.h
        ../include/invariant/AccountSubEntriesCountIsValid.h

        invariant/CacheIsConsistentWithDatabase.cpp
        invariant/InvariantDoesNotHold.cpp
        invariant/InvariantManagerImpl.cpp
        invariant/BucketListIsConsistentWithDatabase.cpp
        invariant/ConservationOfLumens.cpp
        invariant/MinimumAccountBalance.cpp
        invariant/InvariantTestUtils.cpp
        invariant/AccountSubEntriesCountIsValid.cpp
        invariant/LedgerEntryIsValid.cpp)

add_library(invariant SHARED ${INVARIANT_SOURCE_FILES})

set(HISTORY_SOURCE_FILES
        ../include/history/FileTransferInfo.h
        ../include/history/HistoryArchive.h
        ../include/history/HistoryManager.h
        ../include/history/InferredQuorum.h
        ../include/history/StateSnapshot.h
        ../include/history/HistoryTestsUtils.h

        history/FileTransferInfo.cpp
        history/HistoryArchive.cpp
        history/HistoryManagerImpl.cpp
        history/InferredQuorum.cpp
        history/StateSnapshot.cpp
        history/HistoryTestsUtils.cpp
        )

add_library(history SHARED ${HISTORY_SOURCE_FILES})
target_link_libraries(history work historywork bucket)

set(WORK_SOURCE_FILES
        ../include/work/AbstractWork.h
        ../include/work/Work.h
        ../include/work/WorkManager.h
        ../include/work/NtpWork.h

        work/Work.cpp
        work/WorkManagerImpl.cpp
        work/AbstractWork.cpp
        work/NtpWork.cpp
        )
add_library(work SHARED ${WORK_SOURCE_FILES})
target_link_libraries(work util)

set(HISTORYWORK_SOURCES_FILES
        ../include/historywork/BatchDownloadWork.h
        ../include/historywork/BucketDownloadWork.h
        ../include/historywork/FetchRecentQsetsWork.h
        ../include/historywork/GetAndUnzipRemoteFileWork.h
        ../include/historywork/GetHistoryArchiveStateWork.h
        ../include/historywork/GetRemoteFileWork.h
        ../include/historywork/GunzipFileWork.h
        ../include/historywork/GzipFileWork.h
        ../include/historywork/MakeRemoteDirWork.h
        ../include/historywork/Progress.h
        ../include/historywork/PublishWork.h
        ../include/historywork/PutHistoryArchiveStateWork.h
        ../include/historywork/PutRemoteFileWork.h
        ../include/historywork/RepairMissingBucketsWork.h
        ../include/historywork/ResolveSnapshotWork.h
        ../include/historywork/PutSnapshotFilesWork.h
        ../include/historywork/RunCommandWork.h
        ../include/historywork/VerifyBucketWork.h
        ../include/historywork/WriteSnapshotWork.h

        historywork/BatchDownloadWork.cpp
        historywork/BucketDownloadWork.cpp
        historywork/FetchRecentQsetsWork.cpp
        historywork/GetAndUnzipRemoteFileWork.cpp
        historywork/GetHistoryArchiveStateWork.cpp
        historywork/GetRemoteFileWork.cpp
        historywork/GunzipFileWork.cpp
        historywork/GzipFileWork.cpp
        historywork/MakeRemoteDirWork.cpp
        historywork/Progress.cpp
        historywork/PublishWork.cpp
        historywork/PutHistoryArchiveStateWork.cpp
        historywork/PutRemoteFileWork.cpp
        historywork/PutSnapshotFilesWork.cpp
        historywork/RepairMissingBucketsWork.cpp
        historywork/ResolveSnapshotWork.cpp
        historywork/RunCommandWork.cpp
        historywork/VerifyBucketWork.cpp
        historywork/WriteSnapshotWork.cpp
        )

add_library(historywork SHARED ${HISTORYWORK_SOURCES_FILES})

set(HERDER_SOURCE_FILES
        ../include/herder/Herder.h
        ../include/herder/HerderPersistence.h
        ../include/herder/HerderSCPDriver.h
        ../include/herder/HerderUtils.h
        ../include/herder/TxSetFrame.h
        ../include/herder/LedgerCloseData.h
        ../include/herder/Upgrades.h
        herder/Herder.cpp
        herder/HerderImpl.cpp
        herder/HerderPersistenceImpl.cpp
        herder/HerderSCPDriver.cpp
        herder/HerderUtils.cpp
        herder/LedgerCloseData.cpp
        herder/PendingEnvelopes.cpp
        herder/TxSetFrame.cpp
        herder/Upgrades.cpp
        )

add_library(herder SHARED ${HERDER_SOURCE_FILES})
target_link_libraries(herder scp)

set(CATCHUP_SOURCE_FILES
        ../include/catchup/CatchupConfiguration.h
        ../include/catchup/CatchupWork.h
        ../include/catchup/CatchupManager.h
        ../include/catchup/ApplyBucketsWork.h
        ../include/catchup/ApplyLedgerChainWork.h
        ../include/catchup/DownloadBucketsWork.h
        ../include/catchup/VerifyLedgerChainWork.h

        catchup/ApplyBucketsWork.cpp
        catchup/ApplyLedgerChainWork.cpp
        catchup/CatchupConfiguration.cpp
        catchup/CatchupManagerImpl.cpp
        catchup/CatchupWork.cpp
        catchup/DownloadBucketsWork.cpp
        catchup/VerifyLedgerChainWork.cpp
        )

add_library(catchup SHARED ${CATCHUP_SOURCE_FILES})
target_link_libraries(catchup ledger history historywork)

set(PROCESS_SOURCE_FILES
        ../include/process/ProcessManager.h

        process/ProcessManagerImpl.h
        process/ProcessManagerImpl.cpp
        )

add_library(process SHARED ${PROCESS_SOURCE_FILES})


set(SQLITE_SOURCR_FILES
        ../include/sqlite/sqlite3.h
        sqlite/sqlite3.c)

add_library(sqlite SHARED ${SQLITE_SOURCR_FILES})

set(SIMULATION_SOURCE_FILES
        ../include/simulation/LoadGenerator.h
        ../include/simulation/Simulation.h
        ../include/simulation/Topologies.h
        simulation/LoadGenerator.cpp
        simulation/Simulation.cpp
        simulation/Topologies.cpp
        )
add_library(simulation SHARED ${SIMULATION_SOURCE_FILES})
target_link_libraries(simulation transactions ledger herder)

set(APP_SOURCE_FILES
        ../include/application/Application.h
        ../include/application/CommandHandler.h
        ../include/application/ExternalQueue.h
        ../include/application/PersistentState.h
        ../include/application/Config.h

        application/NtpSynchronizationChecker.h
        application/Application.cpp
        application/ApplicationImpl.h
        application/ApplicationImpl.cpp
        application/ExternalQueue.cpp
        application/CommandHandler.cpp
        application/Config.cpp
        application/PersistentState.cpp
        application/NtpSynchronizationChecker.cpp
        application/Maintainer.cpp ../include/application/Maintainer.h)
add_library(application SHARED ${APP_SOURCE_FILES})
target_link_libraries(application
        soci_core util medida jsoncpp
        http overlay catchup process simulation)

set(TESTS_SOURCE_FILES
        ../include/test/test.h
        ../include/test/TestAccount.h
        ../include/test/TestExceptions.h
        ../include/test/TestPrinter.h
        ../include/test/TestUtils.h
        ../include/test/TxTests.h

        test/test.cpp
        test/TestAccount.cpp
        test/TestExceptions.cpp
        test/TestMarket.cpp
        test/TestPrinter.cpp
        test/TxTests.cpp
        test/TestUtils.cpp
        test/TxTests.cpp
        )

add_library(tests SHARED ${TESTS_SOURCE_FILES})
target_link_libraries(tests application bucket)

set(AUTOCHECK_SOURCE_FILES
        ../include/autocheck/apply.hpp
        ../include/autocheck/arbitrary.hpp
        ../include/autocheck/autocheck.hpp
        ../include/autocheck/check.hpp
        ../include/autocheck/classifier.hpp
        ../include/autocheck/distribution.hpp
        ../include/autocheck/function.hpp
        ../include/autocheck/generator.hpp
        ../include/autocheck/generator_combinators.hpp
        ../include/autocheck/is_one_of.hpp
        ../include/autocheck/largest.hpp
        ../include/autocheck/ostream.hpp
        ../include/autocheck/reporter.hpp
        ../include/autocheck/sequence.hpp
        ../include/autocheck/tuple.hpp
        ../include/autocheck/value.hpp
        )

set(CEREAL_SOURCE_FILES
        ../include/cereal/cereal.hpp
        ../include/cereal/macros.hpp
        ../include/cereal/access.hpp
        ../include/cereal/types/base_class.hpp
        ../include/cereal/types/unordered_map.hpp
        ../include/cereal/types/boost_variant.hpp
        ../include/cereal/types/utility.hpp
        ../include/cereal/types/bitset.hpp
        ../include/cereal/types/array.hpp
        ../include/cereal/types/polymorphic.hpp
        ../include/cereal/types/vector.hpp
        ../include/cereal/types/map.hpp
        ../include/cereal/types/complex.hpp
        ../include/cereal/types/tuple.hpp
        ../include/cereal/types/queue.hpp
        ../include/cereal/types/list.hpp
        ../include/cereal/types/forward_list.hpp
        ../include/cereal/types/functional.hpp
        ../include/cereal/types/set.hpp
        ../include/cereal/types/valarray.hpp
        ../include/cereal/types/string.hpp
        ../include/cereal/types/chrono.hpp
        ../include/cereal/types/deque.hpp
        ../include/cereal/types/stack.hpp
        ../include/cereal/types/memory.hpp
        ../include/cereal/types/common.hpp
        ../include/cereal/types/unordered_set.hpp
        ../include/cereal/types/concepts/pair_associative_container.hpp
        ../include/cereal/details/static_object.hpp
        ../include/cereal/details/polymorphic_impl_fwd.hpp
        ../include/cereal/details/traits.hpp
        ../include/cereal/details/polymorphic_impl.hpp
        ../include/cereal/details/helpers.hpp
        ../include/cereal/details/util.hpp
        ../include/cereal/archives/json.hpp
        ../include/cereal/archives/adapters.hpp
        ../include/cereal/archives/xml.hpp
        ../include/cereal/archives/portable_binary.hpp
        ../include/cereal/archives/binary.hpp
        ../include/cereal/external/rapidjson/fwd.h
        ../include/cereal/external/rapidjson/memorystream.h
        ../include/cereal/external/rapidjson/reader.h
        ../include/cereal/external/rapidjson/prettywriter.h
        ../include/cereal/external/rapidjson/ostreamwrapper.h
        ../include/cereal/external/rapidjson/encodedstream.h
        ../include/cereal/external/rapidjson/filereadstream.h
        ../include/cereal/external/rapidjson/istreamwrapper.h
        ../include/cereal/external/rapidjson/stringbuffer.h
        ../include/cereal/external/rapidjson/internal/ieee754.h
        ../include/cereal/external/rapidjson/internal/strtod.h
        ../include/cereal/external/rapidjson/internal/swap.h
        ../include/cereal/external/rapidjson/internal/regex.h
        ../include/cereal/external/rapidjson/internal/diyfp.h
        ../include/cereal/external/rapidjson/internal/biginteger.h
        ../include/cereal/external/rapidjson/internal/strfunc.h
        ../include/cereal/external/rapidjson/internal/itoa.h
        ../include/cereal/external/rapidjson/internal/stack.h
        ../include/cereal/external/rapidjson/internal/dtoa.h
        ../include/cereal/external/rapidjson/internal/meta.h
        ../include/cereal/external/rapidjson/internal/pow10.h
        ../include/cereal/external/rapidjson/encodings.h
        ../include/cereal/external/rapidjson/schema.h
        ../include/cereal/external/rapidjson/stream.h
        ../include/cereal/external/rapidjson/filewritestream.h
        ../include/cereal/external/rapidjson/rapidjson.h
        ../include/cereal/external/rapidjson/document.h
        ../include/cereal/external/rapidjson/allocators.h
        ../include/cereal/external/rapidjson/writer.h
        ../include/cereal/external/rapidjson/error/error.h
        ../include/cereal/external/rapidjson/error/en.h
        ../include/cereal/external/rapidjson/memorybuffer.h
        ../include/cereal/external/rapidjson/pointer.h
        ../include/cereal/external/rapidjson/msinttypes/inttypes.h
        ../include/cereal/external/rapidjson/msinttypes/stdint.h
        ../include/cereal/external/rapidxml/rapidxml_iterators.hpp
        ../include/cereal/external/rapidxml/rapidxml_print.hpp
        ../include/cereal/external/rapidxml/rapidxml_utils.hpp
        ../include/cereal/external/rapidxml/rapidxml.hpp
        ../include/cereal/external/rapidxml/license.txt
        ../include/cereal/external/rapidxml/manual.html
        ../include/cereal/external/base64.hpp
        )

set(VIXAL_CORE_SOURCE_FILES
        ${AUTOCHECK_SOURCE_FILES}
        ${CEREAL_SOURCE_FILES}
        ../include/catch.hpp
        main/dumpxdr.cpp
        main/main.cpp)


set(APPLICATION_NAME vixal-core)

add_executable(${APPLICATION_NAME}
        ${VIXAL_CORE_SOURCE_FILES}
        ${VIXAL_CORE_HEADER_FILES}
        ${CATCH2_SOURCE_FILES}
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
