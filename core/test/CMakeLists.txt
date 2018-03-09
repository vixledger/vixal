MACRO(project_add_test TNAME MNAME)
    add_executable(
            ${TNAME}
            test_main.cpp
            ${MNAME}/${TNAME}.cpp
    )

    target_link_libraries(
            ${TNAME}
            ${MNAME}
            ${ARGN}
    )

    add_test(NAME ${TNAME} COMMAND ${TNAME})

ENDMACRO(project_add_test)


include_directories(
        ${VIXAL_SOURCE_INCLUDE_DIR}
        ${VIXAL_BINARY_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../quirks
)


project_add_test(CryptoTests crypto util)

project_add_test(FsTests util)

project_add_test(BalanceTests util)
project_add_test(BigDivideTests util)
project_add_test(BitsetEnumeratorTests util)
project_add_test(StatusManagerTest util)
project_add_test(LruCacheTests util)
project_add_test(Uint128Tests util)
project_add_test(TimerTests util tests)
project_add_test(abstract_clock_test util)
project_add_test(BucketTests bucket tests history)
project_add_test(DatabaseTests database tests)
project_add_test(UpgradesTests herder tests simulation history)
project_add_test(OverlayManagerTests overlay tests transactions)
project_add_test(FloodTests overlay tests simulation)
project_add_test(OverlayTests overlay tests)
project_add_test(PeerRecordTests overlay tests)
project_add_test(TCPPeerTests overlay tests simulation)
project_add_test(TrackerTests overlay tests)
project_add_test(ItemFetcherTests overlay tests)
project_add_test(LedgerTests ledger tests database)
project_add_test(LedgerHeaderTests ledger tests)
project_add_test(LedgerEntryTests ledger tests)
project_add_test(ConfigTests application tests)
project_add_test(LedgerDeltaTests ledger tests)
# project_add_test(LedgerPerformanceTests ledger)
project_add_test(SyncingLedgerChainTests ledger tests)
project_add_test(HerderTests herder tests simulation ledger transactions)
project_add_test(PendingEnvelopesTests herder tests)
project_add_test(HistoryTests history tests catchup)
project_add_test(InferredQuorumTests history tests)
project_add_test(CatchupWorkTests catchup tests)
project_add_test(SCPTests scp tests)
project_add_test(SCPUnitTests scp tests)
project_add_test(QuorumSetTests scp tests)
project_add_test(AllowTrustTests transactions tests)
project_add_test(ChangeTrustTests transactions tests)
project_add_test(ExchangeTests transactions tests)
project_add_test(InflationTests transactions tests)
project_add_test(ManageDataTests transactions tests)
project_add_test(MergeTests transactions tests)
project_add_test(OfferTests transactions tests)
project_add_test(PathPaymentTests transactions tests xdrpp)
project_add_test(PaymentTests transactions tests)
project_add_test(SetOptionsTests transactions tests)
project_add_test(SignatureUtilsTest transactions tests)
project_add_test(TxEnvelopeTests transactions tests)
project_add_test(TxResultsTests transactions tests)
project_add_test(InvariantTests invariant tests)
project_add_test(MinimumAccountBalanceTests invariant tests)
project_add_test(CacheIsConsistentWithDatabaseTests invariant tests)
project_add_test(BucketListIsConsistentWithDatabaseTests invariant tests)
project_add_test(AccountSubEntriesCountIsValidTests invariant tests)
project_add_test(ConservationOfLumensTests invariant tests)
project_add_test(ProcessTests process tests)


