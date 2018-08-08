// -*- C++ -*-
// Automatically generated from /Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/core/include/xdr/ledger.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR__USERS_LIUWEI_WORKS_BLOCKCHAIN_SRC_GITHUB_COM_VIXLEDGER_VIXAL_CORE_INCLUDE_XDR_LEDGER_H_INCLUDED__
#define __XDR__USERS_LIUWEI_WORKS_BLOCKCHAIN_SRC_GITHUB_COM_VIXLEDGER_VIXAL_CORE_INCLUDE_XDR_LEDGER_H_INCLUDED__ 1

#include <xdrpp/types.h>

#include "xdr/scp.h"
#include "xdr/transaction.h"


namespace vixal {

using UpgradeType = xdr::opaque_vec<128>;

struct VixalValue {
  struct _ext_t {
    using _xdr_case_type = xdr::xdr_traits<int>::case_type;
  private:
    _xdr_case_type v_;
    union {
    };

  public:
    static constexpr const bool _xdr_has_default_case = false;
    static const std::vector<int> &_xdr_case_values() {
      static const std::vector<int> _xdr_disc_vec {
        0
      };
      return _xdr_disc_vec;
    }
    static constexpr int _xdr_field_number(_xdr_case_type which) {
      return which == 0 ? 0
        : -1;
    }
    template<typename _F, typename..._A> static bool
    _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
      switch (_which) {
      case 0:
        return true;
      }
      return false;
    }

    _xdr_case_type _xdr_discriminant() const { return v_; }
    void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
      int fnum = _xdr_field_number(which);
      if (fnum < 0 && validate)
        throw xdr::xdr_bad_discriminant("bad value of v in _ext_t");
      if (fnum != _xdr_field_number(v_)) {
        this->~_ext_t();
        v_ = which;
        _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
      }
      else
        v_ = which;
    }
    explicit _ext_t(std::int32_t which = std::int32_t{}) : v_(which) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
    }
    _ext_t(const _ext_t &source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this, source);
    }
    _ext_t(_ext_t &&source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this,
                        std::move(source));
    }
    ~_ext_t() { _xdr_with_mem_ptr(xdr::field_destructor, v_, *this); }
    _ext_t &operator=(const _ext_t &source) {
      if (_xdr_field_number(v_)
          == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this, source);
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this, source);
      }
      v_ = source.v_;
      return *this;
    }
    _ext_t &operator=(_ext_t &&source) {
      if (_xdr_field_number(v_)
           == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this,
                          std::move(source));
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this,
                          std::move(source));
      }
      v_ = source.v_;
      return *this;
    }

    std::int32_t v() const { return std::int32_t(v_); }
    _ext_t &v(int _xdr_d, bool _xdr_validate = true) {
      _xdr_discriminant(_xdr_d, _xdr_validate);
      return *this;
    }

  };

  Hash txSetHash{};
  uint64 closeTime{};
  xdr::xvector<UpgradeType,6> upgrades{};
  _ext_t ext{};

  VixalValue() = default;
  template<typename _txSetHash_T,
           typename _closeTime_T,
           typename _upgrades_T,
           typename _ext_T,
           typename = typename
           std::enable_if<std::is_constructible<Hash, _txSetHash_T>::value
                          && std::is_constructible<uint64, _closeTime_T>::value
                          && std::is_constructible<xdr::xvector<UpgradeType,6>, _upgrades_T>::value
                          && std::is_constructible<_ext_t, _ext_T>::value
                         >::type>
  explicit VixalValue(_txSetHash_T &&_txSetHash,
                      _closeTime_T &&_closeTime,
                      _upgrades_T &&_upgrades,
                      _ext_T &&_ext)
    : txSetHash(std::forward<_txSetHash_T>(_txSetHash)),
      closeTime(std::forward<_closeTime_T>(_closeTime)),
      upgrades(std::forward<_upgrades_T>(_upgrades)),
      ext(std::forward<_ext_T>(_ext)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::VixalValue::_ext_t> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::VixalValue::_ext_t;
  using case_type = ::vixal::VixalValue::_ext_t::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().v());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::VixalValue::_ext_t &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::VixalValue::_ext_t &obj) {
    xdr::archive(ar, obj.v(), "v");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::VixalValue::_ext_t &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "v");
    obj.v(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::VixalValue>
  : xdr_struct_base<field_ptr<::vixal::VixalValue,
                              decltype(::vixal::VixalValue::txSetHash),
                              &::vixal::VixalValue::txSetHash>,
                    field_ptr<::vixal::VixalValue,
                              decltype(::vixal::VixalValue::closeTime),
                              &::vixal::VixalValue::closeTime>,
                    field_ptr<::vixal::VixalValue,
                              decltype(::vixal::VixalValue::upgrades),
                              &::vixal::VixalValue::upgrades>,
                    field_ptr<::vixal::VixalValue,
                              decltype(::vixal::VixalValue::ext),
                              &::vixal::VixalValue::ext>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::VixalValue &obj) {
    archive(ar, obj.txSetHash, "txSetHash");
    archive(ar, obj.closeTime, "closeTime");
    archive(ar, obj.upgrades, "upgrades");
    archive(ar, obj.ext, "ext");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::VixalValue &obj) {
    archive(ar, obj.txSetHash, "txSetHash");
    archive(ar, obj.closeTime, "closeTime");
    archive(ar, obj.upgrades, "upgrades");
    archive(ar, obj.ext, "ext");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct LedgerHeader {
  struct _ext_t {
    using _xdr_case_type = xdr::xdr_traits<int>::case_type;
  private:
    _xdr_case_type v_;
    union {
    };

  public:
    static constexpr const bool _xdr_has_default_case = false;
    static const std::vector<int> &_xdr_case_values() {
      static const std::vector<int> _xdr_disc_vec {
        0
      };
      return _xdr_disc_vec;
    }
    static constexpr int _xdr_field_number(_xdr_case_type which) {
      return which == 0 ? 0
        : -1;
    }
    template<typename _F, typename..._A> static bool
    _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
      switch (_which) {
      case 0:
        return true;
      }
      return false;
    }

    _xdr_case_type _xdr_discriminant() const { return v_; }
    void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
      int fnum = _xdr_field_number(which);
      if (fnum < 0 && validate)
        throw xdr::xdr_bad_discriminant("bad value of v in _ext_t");
      if (fnum != _xdr_field_number(v_)) {
        this->~_ext_t();
        v_ = which;
        _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
      }
      else
        v_ = which;
    }
    explicit _ext_t(std::int32_t which = std::int32_t{}) : v_(which) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
    }
    _ext_t(const _ext_t &source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this, source);
    }
    _ext_t(_ext_t &&source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this,
                        std::move(source));
    }
    ~_ext_t() { _xdr_with_mem_ptr(xdr::field_destructor, v_, *this); }
    _ext_t &operator=(const _ext_t &source) {
      if (_xdr_field_number(v_)
          == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this, source);
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this, source);
      }
      v_ = source.v_;
      return *this;
    }
    _ext_t &operator=(_ext_t &&source) {
      if (_xdr_field_number(v_)
           == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this,
                          std::move(source));
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this,
                          std::move(source));
      }
      v_ = source.v_;
      return *this;
    }

    std::int32_t v() const { return std::int32_t(v_); }
    _ext_t &v(int _xdr_d, bool _xdr_validate = true) {
      _xdr_discriminant(_xdr_d, _xdr_validate);
      return *this;
    }

  };

  uint32 ledgerVersion{};
  Hash previousLedgerHash{};
  VixalValue scpValue{};
  Hash txSetResultHash{};
  Hash bucketListHash{};
  uint32 ledgerSeq{};
  int64 totalCoins{};
  int64 feePool{};
  uint32 inflationSeq{};
  uint64 idPool{};
  uint32 baseFee{};
  uint32 baseReserve{};
  uint32 maxTxSetSize{};
  xdr::xarray<Hash,4> skipList{};
  _ext_t ext{};

  LedgerHeader() = default;
  template<typename _ledgerVersion_T,
           typename _previousLedgerHash_T,
           typename _scpValue_T,
           typename _txSetResultHash_T,
           typename _bucketListHash_T,
           typename _ledgerSeq_T,
           typename _totalCoins_T,
           typename _feePool_T,
           typename _inflationSeq_T,
           typename _idPool_T,
           typename _baseFee_T,
           typename _baseReserve_T,
           typename _maxTxSetSize_T,
           typename _skipList_T,
           typename _ext_T,
           typename = typename
           std::enable_if<std::is_constructible<uint32, _ledgerVersion_T>::value
                          && std::is_constructible<Hash, _previousLedgerHash_T>::value
                          && std::is_constructible<VixalValue, _scpValue_T>::value
                          && std::is_constructible<Hash, _txSetResultHash_T>::value
                          && std::is_constructible<Hash, _bucketListHash_T>::value
                          && std::is_constructible<uint32, _ledgerSeq_T>::value
                          && std::is_constructible<int64, _totalCoins_T>::value
                          && std::is_constructible<int64, _feePool_T>::value
                          && std::is_constructible<uint32, _inflationSeq_T>::value
                          && std::is_constructible<uint64, _idPool_T>::value
                          && std::is_constructible<uint32, _baseFee_T>::value
                          && std::is_constructible<uint32, _baseReserve_T>::value
                          && std::is_constructible<uint32, _maxTxSetSize_T>::value
                          && std::is_constructible<xdr::xarray<Hash,4>, _skipList_T>::value
                          && std::is_constructible<_ext_t, _ext_T>::value
                         >::type>
  explicit LedgerHeader(_ledgerVersion_T &&_ledgerVersion,
                        _previousLedgerHash_T &&_previousLedgerHash,
                        _scpValue_T &&_scpValue,
                        _txSetResultHash_T &&_txSetResultHash,
                        _bucketListHash_T &&_bucketListHash,
                        _ledgerSeq_T &&_ledgerSeq,
                        _totalCoins_T &&_totalCoins,
                        _feePool_T &&_feePool,
                        _inflationSeq_T &&_inflationSeq,
                        _idPool_T &&_idPool,
                        _baseFee_T &&_baseFee,
                        _baseReserve_T &&_baseReserve,
                        _maxTxSetSize_T &&_maxTxSetSize,
                        _skipList_T &&_skipList,
                        _ext_T &&_ext)
    : ledgerVersion(std::forward<_ledgerVersion_T>(_ledgerVersion)),
      previousLedgerHash(std::forward<_previousLedgerHash_T>(_previousLedgerHash)),
      scpValue(std::forward<_scpValue_T>(_scpValue)),
      txSetResultHash(std::forward<_txSetResultHash_T>(_txSetResultHash)),
      bucketListHash(std::forward<_bucketListHash_T>(_bucketListHash)),
      ledgerSeq(std::forward<_ledgerSeq_T>(_ledgerSeq)),
      totalCoins(std::forward<_totalCoins_T>(_totalCoins)),
      feePool(std::forward<_feePool_T>(_feePool)),
      inflationSeq(std::forward<_inflationSeq_T>(_inflationSeq)),
      idPool(std::forward<_idPool_T>(_idPool)),
      baseFee(std::forward<_baseFee_T>(_baseFee)),
      baseReserve(std::forward<_baseReserve_T>(_baseReserve)),
      maxTxSetSize(std::forward<_maxTxSetSize_T>(_maxTxSetSize)),
      skipList(std::forward<_skipList_T>(_skipList)),
      ext(std::forward<_ext_T>(_ext)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::LedgerHeader::_ext_t> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::LedgerHeader::_ext_t;
  using case_type = ::vixal::LedgerHeader::_ext_t::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().v());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::LedgerHeader::_ext_t &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerHeader::_ext_t &obj) {
    xdr::archive(ar, obj.v(), "v");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerHeader::_ext_t &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "v");
    obj.v(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::LedgerHeader>
  : xdr_struct_base<field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::ledgerVersion),
                              &::vixal::LedgerHeader::ledgerVersion>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::previousLedgerHash),
                              &::vixal::LedgerHeader::previousLedgerHash>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::scpValue),
                              &::vixal::LedgerHeader::scpValue>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::txSetResultHash),
                              &::vixal::LedgerHeader::txSetResultHash>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::bucketListHash),
                              &::vixal::LedgerHeader::bucketListHash>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::ledgerSeq),
                              &::vixal::LedgerHeader::ledgerSeq>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::totalCoins),
                              &::vixal::LedgerHeader::totalCoins>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::feePool),
                              &::vixal::LedgerHeader::feePool>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::inflationSeq),
                              &::vixal::LedgerHeader::inflationSeq>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::idPool),
                              &::vixal::LedgerHeader::idPool>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::baseFee),
                              &::vixal::LedgerHeader::baseFee>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::baseReserve),
                              &::vixal::LedgerHeader::baseReserve>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::maxTxSetSize),
                              &::vixal::LedgerHeader::maxTxSetSize>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::skipList),
                              &::vixal::LedgerHeader::skipList>,
                    field_ptr<::vixal::LedgerHeader,
                              decltype(::vixal::LedgerHeader::ext),
                              &::vixal::LedgerHeader::ext>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerHeader &obj) {
    archive(ar, obj.ledgerVersion, "ledgerVersion");
    archive(ar, obj.previousLedgerHash, "previousLedgerHash");
    archive(ar, obj.scpValue, "scpValue");
    archive(ar, obj.txSetResultHash, "txSetResultHash");
    archive(ar, obj.bucketListHash, "bucketListHash");
    archive(ar, obj.ledgerSeq, "ledgerSeq");
    archive(ar, obj.totalCoins, "totalCoins");
    archive(ar, obj.feePool, "feePool");
    archive(ar, obj.inflationSeq, "inflationSeq");
    archive(ar, obj.idPool, "idPool");
    archive(ar, obj.baseFee, "baseFee");
    archive(ar, obj.baseReserve, "baseReserve");
    archive(ar, obj.maxTxSetSize, "maxTxSetSize");
    archive(ar, obj.skipList, "skipList");
    archive(ar, obj.ext, "ext");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerHeader &obj) {
    archive(ar, obj.ledgerVersion, "ledgerVersion");
    archive(ar, obj.previousLedgerHash, "previousLedgerHash");
    archive(ar, obj.scpValue, "scpValue");
    archive(ar, obj.txSetResultHash, "txSetResultHash");
    archive(ar, obj.bucketListHash, "bucketListHash");
    archive(ar, obj.ledgerSeq, "ledgerSeq");
    archive(ar, obj.totalCoins, "totalCoins");
    archive(ar, obj.feePool, "feePool");
    archive(ar, obj.inflationSeq, "inflationSeq");
    archive(ar, obj.idPool, "idPool");
    archive(ar, obj.baseFee, "baseFee");
    archive(ar, obj.baseReserve, "baseReserve");
    archive(ar, obj.maxTxSetSize, "maxTxSetSize");
    archive(ar, obj.skipList, "skipList");
    archive(ar, obj.ext, "ext");
    xdr::validate(obj);
  }
};
}
namespace vixal {

enum LedgerUpgradeType : std::int32_t {
  LEDGER_UPGRADE_VERSION = 1,
  LEDGER_UPGRADE_BASE_FEE = 2,
  LEDGER_UPGRADE_MAX_TX_SET_SIZE = 3,
  LEDGER_UPGRADE_BASE_RESERVE = 4,
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::LedgerUpgradeType>
  : xdr_integral_base<::vixal::LedgerUpgradeType, std::uint32_t> {
  using case_type = std::int32_t;
  static constexpr const bool is_enum = true;
  static constexpr const bool is_numeric = false;
  static const char *enum_name(::vixal::LedgerUpgradeType val) {
    switch (val) {
    case ::vixal::LEDGER_UPGRADE_VERSION:
      return "LEDGER_UPGRADE_VERSION";
    case ::vixal::LEDGER_UPGRADE_BASE_FEE:
      return "LEDGER_UPGRADE_BASE_FEE";
    case ::vixal::LEDGER_UPGRADE_MAX_TX_SET_SIZE:
      return "LEDGER_UPGRADE_MAX_TX_SET_SIZE";
    case ::vixal::LEDGER_UPGRADE_BASE_RESERVE:
      return "LEDGER_UPGRADE_BASE_RESERVE";
    default:
      return nullptr;
    }
  }
  static const std::vector<int32_t> &enum_values() {
    static const std::vector<int32_t> _xdr_enum_vec = {
      ::vixal::LEDGER_UPGRADE_VERSION,
      ::vixal::LEDGER_UPGRADE_BASE_FEE,
      ::vixal::LEDGER_UPGRADE_MAX_TX_SET_SIZE,
      ::vixal::LEDGER_UPGRADE_BASE_RESERVE
    };
    return _xdr_enum_vec;
  }
};
}
namespace vixal {

struct LedgerUpgrade {
  using _xdr_case_type = xdr::xdr_traits<LedgerUpgradeType>::case_type;
private:
  _xdr_case_type type_;
  union {
    uint32 newLedgerVersion_;
    uint32 newBaseFee_;
    uint32 newMaxTxSetSize_;
    uint32 newBaseReserve_;
  };

public:
  static constexpr const bool _xdr_has_default_case = false;
  static const std::vector<LedgerUpgradeType> &_xdr_case_values() {
    static const std::vector<LedgerUpgradeType> _xdr_disc_vec {
      LEDGER_UPGRADE_VERSION,
      LEDGER_UPGRADE_BASE_FEE,
      LEDGER_UPGRADE_MAX_TX_SET_SIZE,
      LEDGER_UPGRADE_BASE_RESERVE
    };
    return _xdr_disc_vec;
  }
  static constexpr int _xdr_field_number(_xdr_case_type which) {
    return which == LEDGER_UPGRADE_VERSION ? 1
      : which == LEDGER_UPGRADE_BASE_FEE ? 2
      : which == LEDGER_UPGRADE_MAX_TX_SET_SIZE ? 3
      : which == LEDGER_UPGRADE_BASE_RESERVE ? 4
      : -1;
  }
  template<typename _F, typename..._A> static bool
  _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
    switch (_which) {
    case LEDGER_UPGRADE_VERSION:
      _f(&LedgerUpgrade::newLedgerVersion_, std::forward<_A>(_a)...);
      return true;
    case LEDGER_UPGRADE_BASE_FEE:
      _f(&LedgerUpgrade::newBaseFee_, std::forward<_A>(_a)...);
      return true;
    case LEDGER_UPGRADE_MAX_TX_SET_SIZE:
      _f(&LedgerUpgrade::newMaxTxSetSize_, std::forward<_A>(_a)...);
      return true;
    case LEDGER_UPGRADE_BASE_RESERVE:
      _f(&LedgerUpgrade::newBaseReserve_, std::forward<_A>(_a)...);
      return true;
    }
    return false;
  }

  _xdr_case_type _xdr_discriminant() const { return type_; }
  void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of type in LedgerUpgrade");
    if (fnum != _xdr_field_number(type_)) {
      this->~LedgerUpgrade();
      type_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
    }
    else
      type_ = which;
  }
  explicit LedgerUpgrade(LedgerUpgradeType which = LedgerUpgradeType{}) : type_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
  }
  LedgerUpgrade(const LedgerUpgrade &source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this, source);
  }
  LedgerUpgrade(LedgerUpgrade &&source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this,
                      std::move(source));
  }
  ~LedgerUpgrade() { _xdr_with_mem_ptr(xdr::field_destructor, type_, *this); }
  LedgerUpgrade &operator=(const LedgerUpgrade &source) {
    if (_xdr_field_number(type_)
        == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this, source);
    else {
      this->~LedgerUpgrade();
      type_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this, source);
    }
    type_ = source.type_;
    return *this;
  }
  LedgerUpgrade &operator=(LedgerUpgrade &&source) {
    if (_xdr_field_number(type_)
         == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this,
                        std::move(source));
    else {
      this->~LedgerUpgrade();
      type_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this,
                        std::move(source));
    }
    type_ = source.type_;
    return *this;
  }

  LedgerUpgradeType type() const { return LedgerUpgradeType(type_); }
  LedgerUpgrade &type(LedgerUpgradeType _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  uint32 &newLedgerVersion() {
    if (_xdr_field_number(type_) == 1)
      return newLedgerVersion_;
    throw xdr::xdr_wrong_union("LedgerUpgrade: newLedgerVersion accessed when not selected");
  }
  const uint32 &newLedgerVersion() const {
    if (_xdr_field_number(type_) == 1)
      return newLedgerVersion_;
    throw xdr::xdr_wrong_union("LedgerUpgrade: newLedgerVersion accessed when not selected");
  }
  uint32 &newBaseFee() {
    if (_xdr_field_number(type_) == 2)
      return newBaseFee_;
    throw xdr::xdr_wrong_union("LedgerUpgrade: newBaseFee accessed when not selected");
  }
  const uint32 &newBaseFee() const {
    if (_xdr_field_number(type_) == 2)
      return newBaseFee_;
    throw xdr::xdr_wrong_union("LedgerUpgrade: newBaseFee accessed when not selected");
  }
  uint32 &newMaxTxSetSize() {
    if (_xdr_field_number(type_) == 3)
      return newMaxTxSetSize_;
    throw xdr::xdr_wrong_union("LedgerUpgrade: newMaxTxSetSize accessed when not selected");
  }
  const uint32 &newMaxTxSetSize() const {
    if (_xdr_field_number(type_) == 3)
      return newMaxTxSetSize_;
    throw xdr::xdr_wrong_union("LedgerUpgrade: newMaxTxSetSize accessed when not selected");
  }
  uint32 &newBaseReserve() {
    if (_xdr_field_number(type_) == 4)
      return newBaseReserve_;
    throw xdr::xdr_wrong_union("LedgerUpgrade: newBaseReserve accessed when not selected");
  }
  const uint32 &newBaseReserve() const {
    if (_xdr_field_number(type_) == 4)
      return newBaseReserve_;
    throw xdr::xdr_wrong_union("LedgerUpgrade: newBaseReserve accessed when not selected");
  }
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::LedgerUpgrade> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::LedgerUpgrade;
  using case_type = ::vixal::LedgerUpgrade::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().type());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    case 1:
      return "newLedgerVersion";
    case 2:
      return "newBaseFee";
    case 3:
      return "newMaxTxSetSize";
    case 4:
      return "newBaseReserve";
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::LedgerUpgrade &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of type in LedgerUpgrade");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerUpgrade &obj) {
    xdr::archive(ar, obj.type(), "type");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of type in LedgerUpgrade");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerUpgrade &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "type");
    obj.type(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct LedgerKey {
  struct _account_t {
    AccountID accountID{};

    _account_t() = default;
    template<typename _accountID_T,
             typename = typename
             std::enable_if<std::is_constructible<AccountID, _accountID_T>::value
                           >::type>
    explicit _account_t(_accountID_T &&_accountID)
      : accountID(std::forward<_accountID_T>(_accountID)) {}
  };
  struct _trustLine_t {
    AccountID accountID{};
    Asset asset{};

    _trustLine_t() = default;
    template<typename _accountID_T,
             typename _asset_T,
             typename = typename
             std::enable_if<std::is_constructible<AccountID, _accountID_T>::value
                            && std::is_constructible<Asset, _asset_T>::value
                           >::type>
    explicit _trustLine_t(_accountID_T &&_accountID,
                          _asset_T &&_asset)
      : accountID(std::forward<_accountID_T>(_accountID)),
        asset(std::forward<_asset_T>(_asset)) {}
  };
  struct _offer_t {
    AccountID sellerID{};
    uint64 offerID{};

    _offer_t() = default;
    template<typename _sellerID_T,
             typename _offerID_T,
             typename = typename
             std::enable_if<std::is_constructible<AccountID, _sellerID_T>::value
                            && std::is_constructible<uint64, _offerID_T>::value
                           >::type>
    explicit _offer_t(_sellerID_T &&_sellerID,
                      _offerID_T &&_offerID)
      : sellerID(std::forward<_sellerID_T>(_sellerID)),
        offerID(std::forward<_offerID_T>(_offerID)) {}
  };
  struct _data_t {
    AccountID accountID{};
    string64 dataName{};

    _data_t() = default;
    template<typename _accountID_T,
             typename _dataName_T,
             typename = typename
             std::enable_if<std::is_constructible<AccountID, _accountID_T>::value
                            && std::is_constructible<string64, _dataName_T>::value
                           >::type>
    explicit _data_t(_accountID_T &&_accountID,
                     _dataName_T &&_dataName)
      : accountID(std::forward<_accountID_T>(_accountID)),
        dataName(std::forward<_dataName_T>(_dataName)) {}
  };

  using _xdr_case_type = xdr::xdr_traits<LedgerEntryType>::case_type;
private:
  _xdr_case_type type_;
  union {
    _account_t account_;
    _trustLine_t trustLine_;
    _offer_t offer_;
    _data_t data_;
  };

public:
  static constexpr const bool _xdr_has_default_case = false;
  static const std::vector<LedgerEntryType> &_xdr_case_values() {
    static const std::vector<LedgerEntryType> _xdr_disc_vec {
      ACCOUNT,
      TRUSTLINE,
      OFFER,
      DATA
    };
    return _xdr_disc_vec;
  }
  static constexpr int _xdr_field_number(_xdr_case_type which) {
    return which == ACCOUNT ? 1
      : which == TRUSTLINE ? 2
      : which == OFFER ? 3
      : which == DATA ? 4
      : -1;
  }
  template<typename _F, typename..._A> static bool
  _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
    switch (_which) {
    case ACCOUNT:
      _f(&LedgerKey::account_, std::forward<_A>(_a)...);
      return true;
    case TRUSTLINE:
      _f(&LedgerKey::trustLine_, std::forward<_A>(_a)...);
      return true;
    case OFFER:
      _f(&LedgerKey::offer_, std::forward<_A>(_a)...);
      return true;
    case DATA:
      _f(&LedgerKey::data_, std::forward<_A>(_a)...);
      return true;
    }
    return false;
  }

  _xdr_case_type _xdr_discriminant() const { return type_; }
  void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of type in LedgerKey");
    if (fnum != _xdr_field_number(type_)) {
      this->~LedgerKey();
      type_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
    }
    else
      type_ = which;
  }
  explicit LedgerKey(LedgerEntryType which = LedgerEntryType{}) : type_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
  }
  LedgerKey(const LedgerKey &source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this, source);
  }
  LedgerKey(LedgerKey &&source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this,
                      std::move(source));
  }
  ~LedgerKey() { _xdr_with_mem_ptr(xdr::field_destructor, type_, *this); }
  LedgerKey &operator=(const LedgerKey &source) {
    if (_xdr_field_number(type_)
        == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this, source);
    else {
      this->~LedgerKey();
      type_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this, source);
    }
    type_ = source.type_;
    return *this;
  }
  LedgerKey &operator=(LedgerKey &&source) {
    if (_xdr_field_number(type_)
         == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this,
                        std::move(source));
    else {
      this->~LedgerKey();
      type_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this,
                        std::move(source));
    }
    type_ = source.type_;
    return *this;
  }

  LedgerEntryType type() const { return LedgerEntryType(type_); }
  LedgerKey &type(LedgerEntryType _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  _account_t &account() {
    if (_xdr_field_number(type_) == 1)
      return account_;
    throw xdr::xdr_wrong_union("LedgerKey: account accessed when not selected");
  }
  const _account_t &account() const {
    if (_xdr_field_number(type_) == 1)
      return account_;
    throw xdr::xdr_wrong_union("LedgerKey: account accessed when not selected");
  }
  _trustLine_t &trustLine() {
    if (_xdr_field_number(type_) == 2)
      return trustLine_;
    throw xdr::xdr_wrong_union("LedgerKey: trustLine accessed when not selected");
  }
  const _trustLine_t &trustLine() const {
    if (_xdr_field_number(type_) == 2)
      return trustLine_;
    throw xdr::xdr_wrong_union("LedgerKey: trustLine accessed when not selected");
  }
  _offer_t &offer() {
    if (_xdr_field_number(type_) == 3)
      return offer_;
    throw xdr::xdr_wrong_union("LedgerKey: offer accessed when not selected");
  }
  const _offer_t &offer() const {
    if (_xdr_field_number(type_) == 3)
      return offer_;
    throw xdr::xdr_wrong_union("LedgerKey: offer accessed when not selected");
  }
  _data_t &data() {
    if (_xdr_field_number(type_) == 4)
      return data_;
    throw xdr::xdr_wrong_union("LedgerKey: data accessed when not selected");
  }
  const _data_t &data() const {
    if (_xdr_field_number(type_) == 4)
      return data_;
    throw xdr::xdr_wrong_union("LedgerKey: data accessed when not selected");
  }
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::LedgerKey::_account_t>
  : xdr_struct_base<field_ptr<::vixal::LedgerKey::_account_t,
                              decltype(::vixal::LedgerKey::_account_t::accountID),
                              &::vixal::LedgerKey::_account_t::accountID>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerKey::_account_t &obj) {
    archive(ar, obj.accountID, "accountID");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerKey::_account_t &obj) {
    archive(ar, obj.accountID, "accountID");
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::LedgerKey::_trustLine_t>
  : xdr_struct_base<field_ptr<::vixal::LedgerKey::_trustLine_t,
                              decltype(::vixal::LedgerKey::_trustLine_t::accountID),
                              &::vixal::LedgerKey::_trustLine_t::accountID>,
                    field_ptr<::vixal::LedgerKey::_trustLine_t,
                              decltype(::vixal::LedgerKey::_trustLine_t::asset),
                              &::vixal::LedgerKey::_trustLine_t::asset>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerKey::_trustLine_t &obj) {
    archive(ar, obj.accountID, "accountID");
    archive(ar, obj.asset, "asset");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerKey::_trustLine_t &obj) {
    archive(ar, obj.accountID, "accountID");
    archive(ar, obj.asset, "asset");
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::LedgerKey::_offer_t>
  : xdr_struct_base<field_ptr<::vixal::LedgerKey::_offer_t,
                              decltype(::vixal::LedgerKey::_offer_t::sellerID),
                              &::vixal::LedgerKey::_offer_t::sellerID>,
                    field_ptr<::vixal::LedgerKey::_offer_t,
                              decltype(::vixal::LedgerKey::_offer_t::offerID),
                              &::vixal::LedgerKey::_offer_t::offerID>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerKey::_offer_t &obj) {
    archive(ar, obj.sellerID, "sellerID");
    archive(ar, obj.offerID, "offerID");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerKey::_offer_t &obj) {
    archive(ar, obj.sellerID, "sellerID");
    archive(ar, obj.offerID, "offerID");
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::LedgerKey::_data_t>
  : xdr_struct_base<field_ptr<::vixal::LedgerKey::_data_t,
                              decltype(::vixal::LedgerKey::_data_t::accountID),
                              &::vixal::LedgerKey::_data_t::accountID>,
                    field_ptr<::vixal::LedgerKey::_data_t,
                              decltype(::vixal::LedgerKey::_data_t::dataName),
                              &::vixal::LedgerKey::_data_t::dataName>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerKey::_data_t &obj) {
    archive(ar, obj.accountID, "accountID");
    archive(ar, obj.dataName, "dataName");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerKey::_data_t &obj) {
    archive(ar, obj.accountID, "accountID");
    archive(ar, obj.dataName, "dataName");
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::LedgerKey> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::LedgerKey;
  using case_type = ::vixal::LedgerKey::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().type());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    case 1:
      return "account";
    case 2:
      return "trustLine";
    case 3:
      return "offer";
    case 4:
      return "data";
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::LedgerKey &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of type in LedgerKey");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerKey &obj) {
    xdr::archive(ar, obj.type(), "type");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of type in LedgerKey");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerKey &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "type");
    obj.type(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
}
namespace vixal {

enum BucketEntryType : std::int32_t {
  LIVEENTRY = 0,
  DEADENTRY = 1,
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::BucketEntryType>
  : xdr_integral_base<::vixal::BucketEntryType, std::uint32_t> {
  using case_type = std::int32_t;
  static constexpr const bool is_enum = true;
  static constexpr const bool is_numeric = false;
  static const char *enum_name(::vixal::BucketEntryType val) {
    switch (val) {
    case ::vixal::LIVEENTRY:
      return "LIVEENTRY";
    case ::vixal::DEADENTRY:
      return "DEADENTRY";
    default:
      return nullptr;
    }
  }
  static const std::vector<int32_t> &enum_values() {
    static const std::vector<int32_t> _xdr_enum_vec = {
      ::vixal::LIVEENTRY,
      ::vixal::DEADENTRY
    };
    return _xdr_enum_vec;
  }
};
}
namespace vixal {

struct BucketEntry {
  using _xdr_case_type = xdr::xdr_traits<BucketEntryType>::case_type;
private:
  _xdr_case_type type_;
  union {
    LedgerEntry liveEntry_;
    LedgerKey deadEntry_;
  };

public:
  static constexpr const bool _xdr_has_default_case = false;
  static const std::vector<BucketEntryType> &_xdr_case_values() {
    static const std::vector<BucketEntryType> _xdr_disc_vec {
      LIVEENTRY,
      DEADENTRY
    };
    return _xdr_disc_vec;
  }
  static constexpr int _xdr_field_number(_xdr_case_type which) {
    return which == LIVEENTRY ? 1
      : which == DEADENTRY ? 2
      : -1;
  }
  template<typename _F, typename..._A> static bool
  _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
    switch (_which) {
    case LIVEENTRY:
      _f(&BucketEntry::liveEntry_, std::forward<_A>(_a)...);
      return true;
    case DEADENTRY:
      _f(&BucketEntry::deadEntry_, std::forward<_A>(_a)...);
      return true;
    }
    return false;
  }

  _xdr_case_type _xdr_discriminant() const { return type_; }
  void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of type in BucketEntry");
    if (fnum != _xdr_field_number(type_)) {
      this->~BucketEntry();
      type_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
    }
    else
      type_ = which;
  }
  explicit BucketEntry(BucketEntryType which = BucketEntryType{}) : type_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
  }
  BucketEntry(const BucketEntry &source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this, source);
  }
  BucketEntry(BucketEntry &&source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this,
                      std::move(source));
  }
  ~BucketEntry() { _xdr_with_mem_ptr(xdr::field_destructor, type_, *this); }
  BucketEntry &operator=(const BucketEntry &source) {
    if (_xdr_field_number(type_)
        == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this, source);
    else {
      this->~BucketEntry();
      type_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this, source);
    }
    type_ = source.type_;
    return *this;
  }
  BucketEntry &operator=(BucketEntry &&source) {
    if (_xdr_field_number(type_)
         == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this,
                        std::move(source));
    else {
      this->~BucketEntry();
      type_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this,
                        std::move(source));
    }
    type_ = source.type_;
    return *this;
  }

  BucketEntryType type() const { return BucketEntryType(type_); }
  BucketEntry &type(BucketEntryType _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  LedgerEntry &liveEntry() {
    if (_xdr_field_number(type_) == 1)
      return liveEntry_;
    throw xdr::xdr_wrong_union("BucketEntry: liveEntry accessed when not selected");
  }
  const LedgerEntry &liveEntry() const {
    if (_xdr_field_number(type_) == 1)
      return liveEntry_;
    throw xdr::xdr_wrong_union("BucketEntry: liveEntry accessed when not selected");
  }
  LedgerKey &deadEntry() {
    if (_xdr_field_number(type_) == 2)
      return deadEntry_;
    throw xdr::xdr_wrong_union("BucketEntry: deadEntry accessed when not selected");
  }
  const LedgerKey &deadEntry() const {
    if (_xdr_field_number(type_) == 2)
      return deadEntry_;
    throw xdr::xdr_wrong_union("BucketEntry: deadEntry accessed when not selected");
  }
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::BucketEntry> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::BucketEntry;
  using case_type = ::vixal::BucketEntry::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().type());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    case 1:
      return "liveEntry";
    case 2:
      return "deadEntry";
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::BucketEntry &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of type in BucketEntry");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::BucketEntry &obj) {
    xdr::archive(ar, obj.type(), "type");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of type in BucketEntry");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::BucketEntry &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "type");
    obj.type(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct TransactionSet {
  Hash previousLedgerHash{};
  xdr::xvector<TransactionEnvelope> txs{};

  TransactionSet() = default;
  template<typename _previousLedgerHash_T,
           typename _txs_T,
           typename = typename
           std::enable_if<std::is_constructible<Hash, _previousLedgerHash_T>::value
                          && std::is_constructible<xdr::xvector<TransactionEnvelope>, _txs_T>::value
                         >::type>
  explicit TransactionSet(_previousLedgerHash_T &&_previousLedgerHash,
                          _txs_T &&_txs)
    : previousLedgerHash(std::forward<_previousLedgerHash_T>(_previousLedgerHash)),
      txs(std::forward<_txs_T>(_txs)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::TransactionSet>
  : xdr_struct_base<field_ptr<::vixal::TransactionSet,
                              decltype(::vixal::TransactionSet::previousLedgerHash),
                              &::vixal::TransactionSet::previousLedgerHash>,
                    field_ptr<::vixal::TransactionSet,
                              decltype(::vixal::TransactionSet::txs),
                              &::vixal::TransactionSet::txs>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::TransactionSet &obj) {
    archive(ar, obj.previousLedgerHash, "previousLedgerHash");
    archive(ar, obj.txs, "txs");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::TransactionSet &obj) {
    archive(ar, obj.previousLedgerHash, "previousLedgerHash");
    archive(ar, obj.txs, "txs");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct TransactionResultPair {
  Hash transactionHash{};
  TransactionResult result{};

  TransactionResultPair() = default;
  template<typename _transactionHash_T,
           typename _result_T,
           typename = typename
           std::enable_if<std::is_constructible<Hash, _transactionHash_T>::value
                          && std::is_constructible<TransactionResult, _result_T>::value
                         >::type>
  explicit TransactionResultPair(_transactionHash_T &&_transactionHash,
                                 _result_T &&_result)
    : transactionHash(std::forward<_transactionHash_T>(_transactionHash)),
      result(std::forward<_result_T>(_result)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::TransactionResultPair>
  : xdr_struct_base<field_ptr<::vixal::TransactionResultPair,
                              decltype(::vixal::TransactionResultPair::transactionHash),
                              &::vixal::TransactionResultPair::transactionHash>,
                    field_ptr<::vixal::TransactionResultPair,
                              decltype(::vixal::TransactionResultPair::result),
                              &::vixal::TransactionResultPair::result>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::TransactionResultPair &obj) {
    archive(ar, obj.transactionHash, "transactionHash");
    archive(ar, obj.result, "result");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::TransactionResultPair &obj) {
    archive(ar, obj.transactionHash, "transactionHash");
    archive(ar, obj.result, "result");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct TransactionResultSet {
  xdr::xvector<TransactionResultPair> results{};

  TransactionResultSet() = default;
  template<typename _results_T,
           typename = typename
           std::enable_if<std::is_constructible<xdr::xvector<TransactionResultPair>, _results_T>::value
                         >::type>
  explicit TransactionResultSet(_results_T &&_results)
    : results(std::forward<_results_T>(_results)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::TransactionResultSet>
  : xdr_struct_base<field_ptr<::vixal::TransactionResultSet,
                              decltype(::vixal::TransactionResultSet::results),
                              &::vixal::TransactionResultSet::results>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::TransactionResultSet &obj) {
    archive(ar, obj.results, "results");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::TransactionResultSet &obj) {
    archive(ar, obj.results, "results");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct TransactionHistoryEntry {
  struct _ext_t {
    using _xdr_case_type = xdr::xdr_traits<int>::case_type;
  private:
    _xdr_case_type v_;
    union {
    };

  public:
    static constexpr const bool _xdr_has_default_case = false;
    static const std::vector<int> &_xdr_case_values() {
      static const std::vector<int> _xdr_disc_vec {
        0
      };
      return _xdr_disc_vec;
    }
    static constexpr int _xdr_field_number(_xdr_case_type which) {
      return which == 0 ? 0
        : -1;
    }
    template<typename _F, typename..._A> static bool
    _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
      switch (_which) {
      case 0:
        return true;
      }
      return false;
    }

    _xdr_case_type _xdr_discriminant() const { return v_; }
    void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
      int fnum = _xdr_field_number(which);
      if (fnum < 0 && validate)
        throw xdr::xdr_bad_discriminant("bad value of v in _ext_t");
      if (fnum != _xdr_field_number(v_)) {
        this->~_ext_t();
        v_ = which;
        _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
      }
      else
        v_ = which;
    }
    explicit _ext_t(std::int32_t which = std::int32_t{}) : v_(which) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
    }
    _ext_t(const _ext_t &source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this, source);
    }
    _ext_t(_ext_t &&source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this,
                        std::move(source));
    }
    ~_ext_t() { _xdr_with_mem_ptr(xdr::field_destructor, v_, *this); }
    _ext_t &operator=(const _ext_t &source) {
      if (_xdr_field_number(v_)
          == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this, source);
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this, source);
      }
      v_ = source.v_;
      return *this;
    }
    _ext_t &operator=(_ext_t &&source) {
      if (_xdr_field_number(v_)
           == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this,
                          std::move(source));
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this,
                          std::move(source));
      }
      v_ = source.v_;
      return *this;
    }

    std::int32_t v() const { return std::int32_t(v_); }
    _ext_t &v(int _xdr_d, bool _xdr_validate = true) {
      _xdr_discriminant(_xdr_d, _xdr_validate);
      return *this;
    }

  };

  uint32 ledgerSeq{};
  TransactionSet txSet{};
  _ext_t ext{};

  TransactionHistoryEntry() = default;
  template<typename _ledgerSeq_T,
           typename _txSet_T,
           typename _ext_T,
           typename = typename
           std::enable_if<std::is_constructible<uint32, _ledgerSeq_T>::value
                          && std::is_constructible<TransactionSet, _txSet_T>::value
                          && std::is_constructible<_ext_t, _ext_T>::value
                         >::type>
  explicit TransactionHistoryEntry(_ledgerSeq_T &&_ledgerSeq,
                                   _txSet_T &&_txSet,
                                   _ext_T &&_ext)
    : ledgerSeq(std::forward<_ledgerSeq_T>(_ledgerSeq)),
      txSet(std::forward<_txSet_T>(_txSet)),
      ext(std::forward<_ext_T>(_ext)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::TransactionHistoryEntry::_ext_t> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::TransactionHistoryEntry::_ext_t;
  using case_type = ::vixal::TransactionHistoryEntry::_ext_t::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().v());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::TransactionHistoryEntry::_ext_t &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::TransactionHistoryEntry::_ext_t &obj) {
    xdr::archive(ar, obj.v(), "v");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::TransactionHistoryEntry::_ext_t &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "v");
    obj.v(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::TransactionHistoryEntry>
  : xdr_struct_base<field_ptr<::vixal::TransactionHistoryEntry,
                              decltype(::vixal::TransactionHistoryEntry::ledgerSeq),
                              &::vixal::TransactionHistoryEntry::ledgerSeq>,
                    field_ptr<::vixal::TransactionHistoryEntry,
                              decltype(::vixal::TransactionHistoryEntry::txSet),
                              &::vixal::TransactionHistoryEntry::txSet>,
                    field_ptr<::vixal::TransactionHistoryEntry,
                              decltype(::vixal::TransactionHistoryEntry::ext),
                              &::vixal::TransactionHistoryEntry::ext>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::TransactionHistoryEntry &obj) {
    archive(ar, obj.ledgerSeq, "ledgerSeq");
    archive(ar, obj.txSet, "txSet");
    archive(ar, obj.ext, "ext");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::TransactionHistoryEntry &obj) {
    archive(ar, obj.ledgerSeq, "ledgerSeq");
    archive(ar, obj.txSet, "txSet");
    archive(ar, obj.ext, "ext");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct TransactionHistoryResultEntry {
  struct _ext_t {
    using _xdr_case_type = xdr::xdr_traits<int>::case_type;
  private:
    _xdr_case_type v_;
    union {
    };

  public:
    static constexpr const bool _xdr_has_default_case = false;
    static const std::vector<int> &_xdr_case_values() {
      static const std::vector<int> _xdr_disc_vec {
        0
      };
      return _xdr_disc_vec;
    }
    static constexpr int _xdr_field_number(_xdr_case_type which) {
      return which == 0 ? 0
        : -1;
    }
    template<typename _F, typename..._A> static bool
    _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
      switch (_which) {
      case 0:
        return true;
      }
      return false;
    }

    _xdr_case_type _xdr_discriminant() const { return v_; }
    void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
      int fnum = _xdr_field_number(which);
      if (fnum < 0 && validate)
        throw xdr::xdr_bad_discriminant("bad value of v in _ext_t");
      if (fnum != _xdr_field_number(v_)) {
        this->~_ext_t();
        v_ = which;
        _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
      }
      else
        v_ = which;
    }
    explicit _ext_t(std::int32_t which = std::int32_t{}) : v_(which) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
    }
    _ext_t(const _ext_t &source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this, source);
    }
    _ext_t(_ext_t &&source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this,
                        std::move(source));
    }
    ~_ext_t() { _xdr_with_mem_ptr(xdr::field_destructor, v_, *this); }
    _ext_t &operator=(const _ext_t &source) {
      if (_xdr_field_number(v_)
          == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this, source);
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this, source);
      }
      v_ = source.v_;
      return *this;
    }
    _ext_t &operator=(_ext_t &&source) {
      if (_xdr_field_number(v_)
           == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this,
                          std::move(source));
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this,
                          std::move(source));
      }
      v_ = source.v_;
      return *this;
    }

    std::int32_t v() const { return std::int32_t(v_); }
    _ext_t &v(int _xdr_d, bool _xdr_validate = true) {
      _xdr_discriminant(_xdr_d, _xdr_validate);
      return *this;
    }

  };

  uint32 ledgerSeq{};
  TransactionResultSet txResultSet{};
  _ext_t ext{};

  TransactionHistoryResultEntry() = default;
  template<typename _ledgerSeq_T,
           typename _txResultSet_T,
           typename _ext_T,
           typename = typename
           std::enable_if<std::is_constructible<uint32, _ledgerSeq_T>::value
                          && std::is_constructible<TransactionResultSet, _txResultSet_T>::value
                          && std::is_constructible<_ext_t, _ext_T>::value
                         >::type>
  explicit TransactionHistoryResultEntry(_ledgerSeq_T &&_ledgerSeq,
                                         _txResultSet_T &&_txResultSet,
                                         _ext_T &&_ext)
    : ledgerSeq(std::forward<_ledgerSeq_T>(_ledgerSeq)),
      txResultSet(std::forward<_txResultSet_T>(_txResultSet)),
      ext(std::forward<_ext_T>(_ext)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::TransactionHistoryResultEntry::_ext_t> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::TransactionHistoryResultEntry::_ext_t;
  using case_type = ::vixal::TransactionHistoryResultEntry::_ext_t::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().v());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::TransactionHistoryResultEntry::_ext_t &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::TransactionHistoryResultEntry::_ext_t &obj) {
    xdr::archive(ar, obj.v(), "v");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::TransactionHistoryResultEntry::_ext_t &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "v");
    obj.v(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::TransactionHistoryResultEntry>
  : xdr_struct_base<field_ptr<::vixal::TransactionHistoryResultEntry,
                              decltype(::vixal::TransactionHistoryResultEntry::ledgerSeq),
                              &::vixal::TransactionHistoryResultEntry::ledgerSeq>,
                    field_ptr<::vixal::TransactionHistoryResultEntry,
                              decltype(::vixal::TransactionHistoryResultEntry::txResultSet),
                              &::vixal::TransactionHistoryResultEntry::txResultSet>,
                    field_ptr<::vixal::TransactionHistoryResultEntry,
                              decltype(::vixal::TransactionHistoryResultEntry::ext),
                              &::vixal::TransactionHistoryResultEntry::ext>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::TransactionHistoryResultEntry &obj) {
    archive(ar, obj.ledgerSeq, "ledgerSeq");
    archive(ar, obj.txResultSet, "txResultSet");
    archive(ar, obj.ext, "ext");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::TransactionHistoryResultEntry &obj) {
    archive(ar, obj.ledgerSeq, "ledgerSeq");
    archive(ar, obj.txResultSet, "txResultSet");
    archive(ar, obj.ext, "ext");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct LedgerHeaderHistoryEntry {
  struct _ext_t {
    using _xdr_case_type = xdr::xdr_traits<int>::case_type;
  private:
    _xdr_case_type v_;
    union {
    };

  public:
    static constexpr const bool _xdr_has_default_case = false;
    static const std::vector<int> &_xdr_case_values() {
      static const std::vector<int> _xdr_disc_vec {
        0
      };
      return _xdr_disc_vec;
    }
    static constexpr int _xdr_field_number(_xdr_case_type which) {
      return which == 0 ? 0
        : -1;
    }
    template<typename _F, typename..._A> static bool
    _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
      switch (_which) {
      case 0:
        return true;
      }
      return false;
    }

    _xdr_case_type _xdr_discriminant() const { return v_; }
    void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
      int fnum = _xdr_field_number(which);
      if (fnum < 0 && validate)
        throw xdr::xdr_bad_discriminant("bad value of v in _ext_t");
      if (fnum != _xdr_field_number(v_)) {
        this->~_ext_t();
        v_ = which;
        _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
      }
      else
        v_ = which;
    }
    explicit _ext_t(std::int32_t which = std::int32_t{}) : v_(which) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
    }
    _ext_t(const _ext_t &source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this, source);
    }
    _ext_t(_ext_t &&source) : v_(source.v_) {
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this,
                        std::move(source));
    }
    ~_ext_t() { _xdr_with_mem_ptr(xdr::field_destructor, v_, *this); }
    _ext_t &operator=(const _ext_t &source) {
      if (_xdr_field_number(v_)
          == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this, source);
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this, source);
      }
      v_ = source.v_;
      return *this;
    }
    _ext_t &operator=(_ext_t &&source) {
      if (_xdr_field_number(v_)
           == _xdr_field_number(source.v_))
        _xdr_with_mem_ptr(xdr::field_assigner, v_, *this,
                          std::move(source));
      else {
        this->~_ext_t();
        v_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this,
                          std::move(source));
      }
      v_ = source.v_;
      return *this;
    }

    std::int32_t v() const { return std::int32_t(v_); }
    _ext_t &v(int _xdr_d, bool _xdr_validate = true) {
      _xdr_discriminant(_xdr_d, _xdr_validate);
      return *this;
    }

  };

  Hash hash{};
  LedgerHeader header{};
  _ext_t ext{};

  LedgerHeaderHistoryEntry() = default;
  template<typename _hash_T,
           typename _header_T,
           typename _ext_T,
           typename = typename
           std::enable_if<std::is_constructible<Hash, _hash_T>::value
                          && std::is_constructible<LedgerHeader, _header_T>::value
                          && std::is_constructible<_ext_t, _ext_T>::value
                         >::type>
  explicit LedgerHeaderHistoryEntry(_hash_T &&_hash,
                                    _header_T &&_header,
                                    _ext_T &&_ext)
    : hash(std::forward<_hash_T>(_hash)),
      header(std::forward<_header_T>(_header)),
      ext(std::forward<_ext_T>(_ext)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::LedgerHeaderHistoryEntry::_ext_t> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::LedgerHeaderHistoryEntry::_ext_t;
  using case_type = ::vixal::LedgerHeaderHistoryEntry::_ext_t::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().v());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::LedgerHeaderHistoryEntry::_ext_t &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerHeaderHistoryEntry::_ext_t &obj) {
    xdr::archive(ar, obj.v(), "v");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of v in _ext_t");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerHeaderHistoryEntry::_ext_t &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "v");
    obj.v(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::LedgerHeaderHistoryEntry>
  : xdr_struct_base<field_ptr<::vixal::LedgerHeaderHistoryEntry,
                              decltype(::vixal::LedgerHeaderHistoryEntry::hash),
                              &::vixal::LedgerHeaderHistoryEntry::hash>,
                    field_ptr<::vixal::LedgerHeaderHistoryEntry,
                              decltype(::vixal::LedgerHeaderHistoryEntry::header),
                              &::vixal::LedgerHeaderHistoryEntry::header>,
                    field_ptr<::vixal::LedgerHeaderHistoryEntry,
                              decltype(::vixal::LedgerHeaderHistoryEntry::ext),
                              &::vixal::LedgerHeaderHistoryEntry::ext>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerHeaderHistoryEntry &obj) {
    archive(ar, obj.hash, "hash");
    archive(ar, obj.header, "header");
    archive(ar, obj.ext, "ext");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerHeaderHistoryEntry &obj) {
    archive(ar, obj.hash, "hash");
    archive(ar, obj.header, "header");
    archive(ar, obj.ext, "ext");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct LedgerSCPMessages {
  uint32 ledgerSeq{};
  xdr::xvector<SCPEnvelope> messages{};

  LedgerSCPMessages() = default;
  template<typename _ledgerSeq_T,
           typename _messages_T,
           typename = typename
           std::enable_if<std::is_constructible<uint32, _ledgerSeq_T>::value
                          && std::is_constructible<xdr::xvector<SCPEnvelope>, _messages_T>::value
                         >::type>
  explicit LedgerSCPMessages(_ledgerSeq_T &&_ledgerSeq,
                             _messages_T &&_messages)
    : ledgerSeq(std::forward<_ledgerSeq_T>(_ledgerSeq)),
      messages(std::forward<_messages_T>(_messages)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::LedgerSCPMessages>
  : xdr_struct_base<field_ptr<::vixal::LedgerSCPMessages,
                              decltype(::vixal::LedgerSCPMessages::ledgerSeq),
                              &::vixal::LedgerSCPMessages::ledgerSeq>,
                    field_ptr<::vixal::LedgerSCPMessages,
                              decltype(::vixal::LedgerSCPMessages::messages),
                              &::vixal::LedgerSCPMessages::messages>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerSCPMessages &obj) {
    archive(ar, obj.ledgerSeq, "ledgerSeq");
    archive(ar, obj.messages, "messages");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerSCPMessages &obj) {
    archive(ar, obj.ledgerSeq, "ledgerSeq");
    archive(ar, obj.messages, "messages");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct SCPHistoryEntryV0 {
  xdr::xvector<SCPQuorumSet> quorumSets{};
  LedgerSCPMessages ledgerMessages{};

  SCPHistoryEntryV0() = default;
  template<typename _quorumSets_T,
           typename _ledgerMessages_T,
           typename = typename
           std::enable_if<std::is_constructible<xdr::xvector<SCPQuorumSet>, _quorumSets_T>::value
                          && std::is_constructible<LedgerSCPMessages, _ledgerMessages_T>::value
                         >::type>
  explicit SCPHistoryEntryV0(_quorumSets_T &&_quorumSets,
                             _ledgerMessages_T &&_ledgerMessages)
    : quorumSets(std::forward<_quorumSets_T>(_quorumSets)),
      ledgerMessages(std::forward<_ledgerMessages_T>(_ledgerMessages)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::SCPHistoryEntryV0>
  : xdr_struct_base<field_ptr<::vixal::SCPHistoryEntryV0,
                              decltype(::vixal::SCPHistoryEntryV0::quorumSets),
                              &::vixal::SCPHistoryEntryV0::quorumSets>,
                    field_ptr<::vixal::SCPHistoryEntryV0,
                              decltype(::vixal::SCPHistoryEntryV0::ledgerMessages),
                              &::vixal::SCPHistoryEntryV0::ledgerMessages>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPHistoryEntryV0 &obj) {
    archive(ar, obj.quorumSets, "quorumSets");
    archive(ar, obj.ledgerMessages, "ledgerMessages");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPHistoryEntryV0 &obj) {
    archive(ar, obj.quorumSets, "quorumSets");
    archive(ar, obj.ledgerMessages, "ledgerMessages");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct SCPHistoryEntry {
  using _xdr_case_type = xdr::xdr_traits<int>::case_type;
private:
  _xdr_case_type v_;
  union {
    SCPHistoryEntryV0 v0_;
  };

public:
  static constexpr const bool _xdr_has_default_case = false;
  static const std::vector<int> &_xdr_case_values() {
    static const std::vector<int> _xdr_disc_vec {
      0
    };
    return _xdr_disc_vec;
  }
  static constexpr int _xdr_field_number(_xdr_case_type which) {
    return which == 0 ? 1
      : -1;
  }
  template<typename _F, typename..._A> static bool
  _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
    switch (_which) {
    case 0:
      _f(&SCPHistoryEntry::v0_, std::forward<_A>(_a)...);
      return true;
    }
    return false;
  }

  _xdr_case_type _xdr_discriminant() const { return v_; }
  void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of v in SCPHistoryEntry");
    if (fnum != _xdr_field_number(v_)) {
      this->~SCPHistoryEntry();
      v_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
    }
    else
      v_ = which;
  }
  explicit SCPHistoryEntry(std::int32_t which = std::int32_t{}) : v_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
  }
  SCPHistoryEntry(const SCPHistoryEntry &source) : v_(source.v_) {
    _xdr_with_mem_ptr(xdr::field_constructor, v_, *this, source);
  }
  SCPHistoryEntry(SCPHistoryEntry &&source) : v_(source.v_) {
    _xdr_with_mem_ptr(xdr::field_constructor, v_, *this,
                      std::move(source));
  }
  ~SCPHistoryEntry() { _xdr_with_mem_ptr(xdr::field_destructor, v_, *this); }
  SCPHistoryEntry &operator=(const SCPHistoryEntry &source) {
    if (_xdr_field_number(v_)
        == _xdr_field_number(source.v_))
      _xdr_with_mem_ptr(xdr::field_assigner, v_, *this, source);
    else {
      this->~SCPHistoryEntry();
      v_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this, source);
    }
    v_ = source.v_;
    return *this;
  }
  SCPHistoryEntry &operator=(SCPHistoryEntry &&source) {
    if (_xdr_field_number(v_)
         == _xdr_field_number(source.v_))
      _xdr_with_mem_ptr(xdr::field_assigner, v_, *this,
                        std::move(source));
    else {
      this->~SCPHistoryEntry();
      v_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this,
                        std::move(source));
    }
    v_ = source.v_;
    return *this;
  }

  std::int32_t v() const { return std::int32_t(v_); }
  SCPHistoryEntry &v(int _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  SCPHistoryEntryV0 &v0() {
    if (_xdr_field_number(v_) == 1)
      return v0_;
    throw xdr::xdr_wrong_union("SCPHistoryEntry: v0 accessed when not selected");
  }
  const SCPHistoryEntryV0 &v0() const {
    if (_xdr_field_number(v_) == 1)
      return v0_;
    throw xdr::xdr_wrong_union("SCPHistoryEntry: v0 accessed when not selected");
  }
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::SCPHistoryEntry> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::SCPHistoryEntry;
  using case_type = ::vixal::SCPHistoryEntry::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().v());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    case 1:
      return "v0";
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::SCPHistoryEntry &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of v in SCPHistoryEntry");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPHistoryEntry &obj) {
    xdr::archive(ar, obj.v(), "v");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of v in SCPHistoryEntry");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPHistoryEntry &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "v");
    obj.v(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
}
namespace vixal {

enum LedgerEntryChangeType : std::int32_t {
  LEDGER_ENTRY_CREATED = 0,
  LEDGER_ENTRY_UPDATED = 1,
  LEDGER_ENTRY_REMOVED = 2,
  LEDGER_ENTRY_STATE = 3,
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::LedgerEntryChangeType>
  : xdr_integral_base<::vixal::LedgerEntryChangeType, std::uint32_t> {
  using case_type = std::int32_t;
  static constexpr const bool is_enum = true;
  static constexpr const bool is_numeric = false;
  static const char *enum_name(::vixal::LedgerEntryChangeType val) {
    switch (val) {
    case ::vixal::LEDGER_ENTRY_CREATED:
      return "LEDGER_ENTRY_CREATED";
    case ::vixal::LEDGER_ENTRY_UPDATED:
      return "LEDGER_ENTRY_UPDATED";
    case ::vixal::LEDGER_ENTRY_REMOVED:
      return "LEDGER_ENTRY_REMOVED";
    case ::vixal::LEDGER_ENTRY_STATE:
      return "LEDGER_ENTRY_STATE";
    default:
      return nullptr;
    }
  }
  static const std::vector<int32_t> &enum_values() {
    static const std::vector<int32_t> _xdr_enum_vec = {
      ::vixal::LEDGER_ENTRY_CREATED,
      ::vixal::LEDGER_ENTRY_UPDATED,
      ::vixal::LEDGER_ENTRY_REMOVED,
      ::vixal::LEDGER_ENTRY_STATE
    };
    return _xdr_enum_vec;
  }
};
}
namespace vixal {

struct LedgerEntryChange {
  using _xdr_case_type = xdr::xdr_traits<LedgerEntryChangeType>::case_type;
private:
  _xdr_case_type type_;
  union {
    LedgerEntry created_;
    LedgerEntry updated_;
    LedgerKey removed_;
    LedgerEntry state_;
  };

public:
  static constexpr const bool _xdr_has_default_case = false;
  static const std::vector<LedgerEntryChangeType> &_xdr_case_values() {
    static const std::vector<LedgerEntryChangeType> _xdr_disc_vec {
      LEDGER_ENTRY_CREATED,
      LEDGER_ENTRY_UPDATED,
      LEDGER_ENTRY_REMOVED,
      LEDGER_ENTRY_STATE
    };
    return _xdr_disc_vec;
  }
  static constexpr int _xdr_field_number(_xdr_case_type which) {
    return which == LEDGER_ENTRY_CREATED ? 1
      : which == LEDGER_ENTRY_UPDATED ? 2
      : which == LEDGER_ENTRY_REMOVED ? 3
      : which == LEDGER_ENTRY_STATE ? 4
      : -1;
  }
  template<typename _F, typename..._A> static bool
  _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
    switch (_which) {
    case LEDGER_ENTRY_CREATED:
      _f(&LedgerEntryChange::created_, std::forward<_A>(_a)...);
      return true;
    case LEDGER_ENTRY_UPDATED:
      _f(&LedgerEntryChange::updated_, std::forward<_A>(_a)...);
      return true;
    case LEDGER_ENTRY_REMOVED:
      _f(&LedgerEntryChange::removed_, std::forward<_A>(_a)...);
      return true;
    case LEDGER_ENTRY_STATE:
      _f(&LedgerEntryChange::state_, std::forward<_A>(_a)...);
      return true;
    }
    return false;
  }

  _xdr_case_type _xdr_discriminant() const { return type_; }
  void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of type in LedgerEntryChange");
    if (fnum != _xdr_field_number(type_)) {
      this->~LedgerEntryChange();
      type_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
    }
    else
      type_ = which;
  }
  explicit LedgerEntryChange(LedgerEntryChangeType which = LedgerEntryChangeType{}) : type_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
  }
  LedgerEntryChange(const LedgerEntryChange &source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this, source);
  }
  LedgerEntryChange(LedgerEntryChange &&source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this,
                      std::move(source));
  }
  ~LedgerEntryChange() { _xdr_with_mem_ptr(xdr::field_destructor, type_, *this); }
  LedgerEntryChange &operator=(const LedgerEntryChange &source) {
    if (_xdr_field_number(type_)
        == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this, source);
    else {
      this->~LedgerEntryChange();
      type_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this, source);
    }
    type_ = source.type_;
    return *this;
  }
  LedgerEntryChange &operator=(LedgerEntryChange &&source) {
    if (_xdr_field_number(type_)
         == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this,
                        std::move(source));
    else {
      this->~LedgerEntryChange();
      type_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this,
                        std::move(source));
    }
    type_ = source.type_;
    return *this;
  }

  LedgerEntryChangeType type() const { return LedgerEntryChangeType(type_); }
  LedgerEntryChange &type(LedgerEntryChangeType _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  LedgerEntry &created() {
    if (_xdr_field_number(type_) == 1)
      return created_;
    throw xdr::xdr_wrong_union("LedgerEntryChange: created accessed when not selected");
  }
  const LedgerEntry &created() const {
    if (_xdr_field_number(type_) == 1)
      return created_;
    throw xdr::xdr_wrong_union("LedgerEntryChange: created accessed when not selected");
  }
  LedgerEntry &updated() {
    if (_xdr_field_number(type_) == 2)
      return updated_;
    throw xdr::xdr_wrong_union("LedgerEntryChange: updated accessed when not selected");
  }
  const LedgerEntry &updated() const {
    if (_xdr_field_number(type_) == 2)
      return updated_;
    throw xdr::xdr_wrong_union("LedgerEntryChange: updated accessed when not selected");
  }
  LedgerKey &removed() {
    if (_xdr_field_number(type_) == 3)
      return removed_;
    throw xdr::xdr_wrong_union("LedgerEntryChange: removed accessed when not selected");
  }
  const LedgerKey &removed() const {
    if (_xdr_field_number(type_) == 3)
      return removed_;
    throw xdr::xdr_wrong_union("LedgerEntryChange: removed accessed when not selected");
  }
  LedgerEntry &state() {
    if (_xdr_field_number(type_) == 4)
      return state_;
    throw xdr::xdr_wrong_union("LedgerEntryChange: state accessed when not selected");
  }
  const LedgerEntry &state() const {
    if (_xdr_field_number(type_) == 4)
      return state_;
    throw xdr::xdr_wrong_union("LedgerEntryChange: state accessed when not selected");
  }
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::LedgerEntryChange> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::LedgerEntryChange;
  using case_type = ::vixal::LedgerEntryChange::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().type());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    case 1:
      return "created";
    case 2:
      return "updated";
    case 3:
      return "removed";
    case 4:
      return "state";
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::LedgerEntryChange &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of type in LedgerEntryChange");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::LedgerEntryChange &obj) {
    xdr::archive(ar, obj.type(), "type");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of type in LedgerEntryChange");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::LedgerEntryChange &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "type");
    obj.type(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
}
namespace vixal {

using LedgerEntryChanges = xdr::xvector<LedgerEntryChange>;

struct OperationMeta {
  LedgerEntryChanges changes{};

  OperationMeta() = default;
  template<typename _changes_T,
           typename = typename
           std::enable_if<std::is_constructible<LedgerEntryChanges, _changes_T>::value
                         >::type>
  explicit OperationMeta(_changes_T &&_changes)
    : changes(std::forward<_changes_T>(_changes)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::OperationMeta>
  : xdr_struct_base<field_ptr<::vixal::OperationMeta,
                              decltype(::vixal::OperationMeta::changes),
                              &::vixal::OperationMeta::changes>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::OperationMeta &obj) {
    archive(ar, obj.changes, "changes");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::OperationMeta &obj) {
    archive(ar, obj.changes, "changes");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct TransactionMetaV1 {
  LedgerEntryChanges txChanges{};
  xdr::xvector<OperationMeta> operations{};

  TransactionMetaV1() = default;
  template<typename _txChanges_T,
           typename _operations_T,
           typename = typename
           std::enable_if<std::is_constructible<LedgerEntryChanges, _txChanges_T>::value
                          && std::is_constructible<xdr::xvector<OperationMeta>, _operations_T>::value
                         >::type>
  explicit TransactionMetaV1(_txChanges_T &&_txChanges,
                             _operations_T &&_operations)
    : txChanges(std::forward<_txChanges_T>(_txChanges)),
      operations(std::forward<_operations_T>(_operations)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::TransactionMetaV1>
  : xdr_struct_base<field_ptr<::vixal::TransactionMetaV1,
                              decltype(::vixal::TransactionMetaV1::txChanges),
                              &::vixal::TransactionMetaV1::txChanges>,
                    field_ptr<::vixal::TransactionMetaV1,
                              decltype(::vixal::TransactionMetaV1::operations),
                              &::vixal::TransactionMetaV1::operations>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::TransactionMetaV1 &obj) {
    archive(ar, obj.txChanges, "txChanges");
    archive(ar, obj.operations, "operations");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::TransactionMetaV1 &obj) {
    archive(ar, obj.txChanges, "txChanges");
    archive(ar, obj.operations, "operations");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct TransactionMeta {
  using _xdr_case_type = xdr::xdr_traits<int>::case_type;
private:
  _xdr_case_type v_;
  union {
    xdr::xvector<OperationMeta> operations_;
    TransactionMetaV1 v1_;
  };

public:
  static constexpr const bool _xdr_has_default_case = false;
  static const std::vector<int> &_xdr_case_values() {
    static const std::vector<int> _xdr_disc_vec {
      0,
      1
    };
    return _xdr_disc_vec;
  }
  static constexpr int _xdr_field_number(_xdr_case_type which) {
    return which == 0 ? 1
      : which == 1 ? 2
      : -1;
  }
  template<typename _F, typename..._A> static bool
  _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
    switch (_which) {
    case 0:
      _f(&TransactionMeta::operations_, std::forward<_A>(_a)...);
      return true;
    case 1:
      _f(&TransactionMeta::v1_, std::forward<_A>(_a)...);
      return true;
    }
    return false;
  }

  _xdr_case_type _xdr_discriminant() const { return v_; }
  void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of v in TransactionMeta");
    if (fnum != _xdr_field_number(v_)) {
      this->~TransactionMeta();
      v_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
    }
    else
      v_ = which;
  }
  explicit TransactionMeta(std::int32_t which = std::int32_t{}) : v_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, v_, *this);
  }
  TransactionMeta(const TransactionMeta &source) : v_(source.v_) {
    _xdr_with_mem_ptr(xdr::field_constructor, v_, *this, source);
  }
  TransactionMeta(TransactionMeta &&source) : v_(source.v_) {
    _xdr_with_mem_ptr(xdr::field_constructor, v_, *this,
                      std::move(source));
  }
  ~TransactionMeta() { _xdr_with_mem_ptr(xdr::field_destructor, v_, *this); }
  TransactionMeta &operator=(const TransactionMeta &source) {
    if (_xdr_field_number(v_)
        == _xdr_field_number(source.v_))
      _xdr_with_mem_ptr(xdr::field_assigner, v_, *this, source);
    else {
      this->~TransactionMeta();
      v_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this, source);
    }
    v_ = source.v_;
    return *this;
  }
  TransactionMeta &operator=(TransactionMeta &&source) {
    if (_xdr_field_number(v_)
         == _xdr_field_number(source.v_))
      _xdr_with_mem_ptr(xdr::field_assigner, v_, *this,
                        std::move(source));
    else {
      this->~TransactionMeta();
      v_ = std::numeric_limits<_xdr_case_type>::max();
      _xdr_with_mem_ptr(xdr::field_constructor, source.v_, *this,
                        std::move(source));
    }
    v_ = source.v_;
    return *this;
  }

  std::int32_t v() const { return std::int32_t(v_); }
  TransactionMeta &v(int _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  xdr::xvector<OperationMeta> &operations() {
    if (_xdr_field_number(v_) == 1)
      return operations_;
    throw xdr::xdr_wrong_union("TransactionMeta: operations accessed when not selected");
  }
  const xdr::xvector<OperationMeta> &operations() const {
    if (_xdr_field_number(v_) == 1)
      return operations_;
    throw xdr::xdr_wrong_union("TransactionMeta: operations accessed when not selected");
  }
  TransactionMetaV1 &v1() {
    if (_xdr_field_number(v_) == 2)
      return v1_;
    throw xdr::xdr_wrong_union("TransactionMeta: v1 accessed when not selected");
  }
  const TransactionMetaV1 &v1() const {
    if (_xdr_field_number(v_) == 2)
      return v1_;
    throw xdr::xdr_wrong_union("TransactionMeta: v1 accessed when not selected");
  }
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::TransactionMeta> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::TransactionMeta;
  using case_type = ::vixal::TransactionMeta::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().v());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    case 1:
      return "operations";
    case 2:
      return "v1";
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::TransactionMeta &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of v in TransactionMeta");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::TransactionMeta &obj) {
    xdr::archive(ar, obj.v(), "v");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of v in TransactionMeta");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::TransactionMeta &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "v");
    obj.v(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.v(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
}
namespace vixal {

}

#endif // !__XDR__USERS_LIUWEI_WORKS_BLOCKCHAIN_SRC_GITHUB_COM_VIXLEDGER_VIXAL_CORE_INCLUDE_XDR_LEDGER_H_INCLUDED__
