// -*- C++ -*-
// Automatically generated from /Users/liuwei/works/blockchain/src/github.com/vixledger/vixal/core/include/xdr/scp.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR__USERS_LIUWEI_WORKS_BLOCKCHAIN_SRC_GITHUB_COM_VIXLEDGER_VIXAL_CORE_INCLUDE_XDR_SCP_H_INCLUDED__
#define __XDR__USERS_LIUWEI_WORKS_BLOCKCHAIN_SRC_GITHUB_COM_VIXLEDGER_VIXAL_CORE_INCLUDE_XDR_SCP_H_INCLUDED__ 1

#include <xdrpp/types.h>

#include "xdr/types.h"


namespace vixal {

using Value = xdr::opaque_vec<>;

struct SCPBallot {
  uint32 counter{};
  Value value{};

  SCPBallot() = default;
  template<typename _counter_T,
           typename _value_T,
           typename = typename
           std::enable_if<std::is_constructible<uint32, _counter_T>::value
                          && std::is_constructible<Value, _value_T>::value
                         >::type>
  explicit SCPBallot(_counter_T &&_counter,
                     _value_T &&_value)
    : counter(std::forward<_counter_T>(_counter)),
      value(std::forward<_value_T>(_value)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::SCPBallot>
  : xdr_struct_base<field_ptr<::vixal::SCPBallot,
                              decltype(::vixal::SCPBallot::counter),
                              &::vixal::SCPBallot::counter>,
                    field_ptr<::vixal::SCPBallot,
                              decltype(::vixal::SCPBallot::value),
                              &::vixal::SCPBallot::value>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPBallot &obj) {
    archive(ar, obj.counter, "counter");
    archive(ar, obj.value, "value");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPBallot &obj) {
    archive(ar, obj.counter, "counter");
    archive(ar, obj.value, "value");
    xdr::validate(obj);
  }
};
}
namespace vixal {

enum SCPStatementType : std::int32_t {
  SCP_ST_PREPARE = 0,
  SCP_ST_CONFIRM = 1,
  SCP_ST_EXTERNALIZE = 2,
  SCP_ST_NOMINATE = 3,
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::SCPStatementType>
  : xdr_integral_base<::vixal::SCPStatementType, std::uint32_t> {
  using case_type = std::int32_t;
  static constexpr const bool is_enum = true;
  static constexpr const bool is_numeric = false;
  static const char *enum_name(::vixal::SCPStatementType val) {
    switch (val) {
    case ::vixal::SCP_ST_PREPARE:
      return "SCP_ST_PREPARE";
    case ::vixal::SCP_ST_CONFIRM:
      return "SCP_ST_CONFIRM";
    case ::vixal::SCP_ST_EXTERNALIZE:
      return "SCP_ST_EXTERNALIZE";
    case ::vixal::SCP_ST_NOMINATE:
      return "SCP_ST_NOMINATE";
    default:
      return nullptr;
    }
  }
  static const std::vector<int32_t> &enum_values() {
    static const std::vector<int32_t> _xdr_enum_vec = {
      ::vixal::SCP_ST_PREPARE,
      ::vixal::SCP_ST_CONFIRM,
      ::vixal::SCP_ST_EXTERNALIZE,
      ::vixal::SCP_ST_NOMINATE
    };
    return _xdr_enum_vec;
  }
};
}
namespace vixal {

struct SCPNomination {
  Hash quorumSetHash{};
  xdr::xvector<Value> votes{};
  xdr::xvector<Value> accepted{};

  SCPNomination() = default;
  template<typename _quorumSetHash_T,
           typename _votes_T,
           typename _accepted_T,
           typename = typename
           std::enable_if<std::is_constructible<Hash, _quorumSetHash_T>::value
                          && std::is_constructible<xdr::xvector<Value>, _votes_T>::value
                          && std::is_constructible<xdr::xvector<Value>, _accepted_T>::value
                         >::type>
  explicit SCPNomination(_quorumSetHash_T &&_quorumSetHash,
                         _votes_T &&_votes,
                         _accepted_T &&_accepted)
    : quorumSetHash(std::forward<_quorumSetHash_T>(_quorumSetHash)),
      votes(std::forward<_votes_T>(_votes)),
      accepted(std::forward<_accepted_T>(_accepted)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::SCPNomination>
  : xdr_struct_base<field_ptr<::vixal::SCPNomination,
                              decltype(::vixal::SCPNomination::quorumSetHash),
                              &::vixal::SCPNomination::quorumSetHash>,
                    field_ptr<::vixal::SCPNomination,
                              decltype(::vixal::SCPNomination::votes),
                              &::vixal::SCPNomination::votes>,
                    field_ptr<::vixal::SCPNomination,
                              decltype(::vixal::SCPNomination::accepted),
                              &::vixal::SCPNomination::accepted>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPNomination &obj) {
    archive(ar, obj.quorumSetHash, "quorumSetHash");
    archive(ar, obj.votes, "votes");
    archive(ar, obj.accepted, "accepted");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPNomination &obj) {
    archive(ar, obj.quorumSetHash, "quorumSetHash");
    archive(ar, obj.votes, "votes");
    archive(ar, obj.accepted, "accepted");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct SCPStatement {
  struct _pledges_t {
    struct _prepare_t {
      Hash quorumSetHash{};
      SCPBallot ballot{};
      xdr::pointer<SCPBallot> prepared{};
      xdr::pointer<SCPBallot> preparedPrime{};
      uint32 nC{};
      uint32 nH{};

      _prepare_t() = default;
      template<typename _quorumSetHash_T,
               typename _ballot_T,
               typename _prepared_T,
               typename _preparedPrime_T,
               typename _nC_T,
               typename _nH_T,
               typename = typename
               std::enable_if<std::is_constructible<Hash, _quorumSetHash_T>::value
                              && std::is_constructible<SCPBallot, _ballot_T>::value
                              && std::is_constructible<xdr::pointer<SCPBallot>, _prepared_T>::value
                              && std::is_constructible<xdr::pointer<SCPBallot>, _preparedPrime_T>::value
                              && std::is_constructible<uint32, _nC_T>::value
                              && std::is_constructible<uint32, _nH_T>::value
                             >::type>
      explicit _prepare_t(_quorumSetHash_T &&_quorumSetHash,
                          _ballot_T &&_ballot,
                          _prepared_T &&_prepared,
                          _preparedPrime_T &&_preparedPrime,
                          _nC_T &&_nC,
                          _nH_T &&_nH)
        : quorumSetHash(std::forward<_quorumSetHash_T>(_quorumSetHash)),
          ballot(std::forward<_ballot_T>(_ballot)),
          prepared(std::forward<_prepared_T>(_prepared)),
          preparedPrime(std::forward<_preparedPrime_T>(_preparedPrime)),
          nC(std::forward<_nC_T>(_nC)),
          nH(std::forward<_nH_T>(_nH)) {}
    };
    struct _confirm_t {
      SCPBallot ballot{};
      uint32 nPrepared{};
      uint32 nCommit{};
      uint32 nH{};
      Hash quorumSetHash{};

      _confirm_t() = default;
      template<typename _ballot_T,
               typename _nPrepared_T,
               typename _nCommit_T,
               typename _nH_T,
               typename _quorumSetHash_T,
               typename = typename
               std::enable_if<std::is_constructible<SCPBallot, _ballot_T>::value
                              && std::is_constructible<uint32, _nPrepared_T>::value
                              && std::is_constructible<uint32, _nCommit_T>::value
                              && std::is_constructible<uint32, _nH_T>::value
                              && std::is_constructible<Hash, _quorumSetHash_T>::value
                             >::type>
      explicit _confirm_t(_ballot_T &&_ballot,
                          _nPrepared_T &&_nPrepared,
                          _nCommit_T &&_nCommit,
                          _nH_T &&_nH,
                          _quorumSetHash_T &&_quorumSetHash)
        : ballot(std::forward<_ballot_T>(_ballot)),
          nPrepared(std::forward<_nPrepared_T>(_nPrepared)),
          nCommit(std::forward<_nCommit_T>(_nCommit)),
          nH(std::forward<_nH_T>(_nH)),
          quorumSetHash(std::forward<_quorumSetHash_T>(_quorumSetHash)) {}
    };
    struct _externalize_t {
      SCPBallot commit{};
      uint32 nH{};
      Hash commitQuorumSetHash{};

      _externalize_t() = default;
      template<typename _commit_T,
               typename _nH_T,
               typename _commitQuorumSetHash_T,
               typename = typename
               std::enable_if<std::is_constructible<SCPBallot, _commit_T>::value
                              && std::is_constructible<uint32, _nH_T>::value
                              && std::is_constructible<Hash, _commitQuorumSetHash_T>::value
                             >::type>
      explicit _externalize_t(_commit_T &&_commit,
                              _nH_T &&_nH,
                              _commitQuorumSetHash_T &&_commitQuorumSetHash)
        : commit(std::forward<_commit_T>(_commit)),
          nH(std::forward<_nH_T>(_nH)),
          commitQuorumSetHash(std::forward<_commitQuorumSetHash_T>(_commitQuorumSetHash)) {}
    };

    using _xdr_case_type = xdr::xdr_traits<SCPStatementType>::case_type;
  private:
    _xdr_case_type type_;
    union {
      _prepare_t prepare_;
      _confirm_t confirm_;
      _externalize_t externalize_;
      SCPNomination nominate_;
    };

  public:
    static constexpr const bool _xdr_has_default_case = false;
    static const std::vector<SCPStatementType> &_xdr_case_values() {
      static const std::vector<SCPStatementType> _xdr_disc_vec {
        SCP_ST_PREPARE,
        SCP_ST_CONFIRM,
        SCP_ST_EXTERNALIZE,
        SCP_ST_NOMINATE
      };
      return _xdr_disc_vec;
    }
    static constexpr int _xdr_field_number(_xdr_case_type which) {
      return which == SCP_ST_PREPARE ? 1
        : which == SCP_ST_CONFIRM ? 2
        : which == SCP_ST_EXTERNALIZE ? 3
        : which == SCP_ST_NOMINATE ? 4
        : -1;
    }
    template<typename _F, typename..._A> static bool
    _xdr_with_mem_ptr(_F &_f, _xdr_case_type _which, _A&&..._a) {
      switch (_which) {
      case SCP_ST_PREPARE:
        _f(&_pledges_t::prepare_, std::forward<_A>(_a)...);
        return true;
      case SCP_ST_CONFIRM:
        _f(&_pledges_t::confirm_, std::forward<_A>(_a)...);
        return true;
      case SCP_ST_EXTERNALIZE:
        _f(&_pledges_t::externalize_, std::forward<_A>(_a)...);
        return true;
      case SCP_ST_NOMINATE:
        _f(&_pledges_t::nominate_, std::forward<_A>(_a)...);
        return true;
      }
      return false;
    }

    _xdr_case_type _xdr_discriminant() const { return type_; }
    void _xdr_discriminant(_xdr_case_type which, bool validate = true) {
      int fnum = _xdr_field_number(which);
      if (fnum < 0 && validate)
        throw xdr::xdr_bad_discriminant("bad value of type in _pledges_t");
      if (fnum != _xdr_field_number(type_)) {
        this->~_pledges_t();
        type_ = which;
        _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
      }
      else
        type_ = which;
    }
    explicit _pledges_t(SCPStatementType which = SCPStatementType{}) : type_(which) {
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
    }
    _pledges_t(const _pledges_t &source) : type_(source.type_) {
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this, source);
    }
    _pledges_t(_pledges_t &&source) : type_(source.type_) {
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this,
                        std::move(source));
    }
    ~_pledges_t() { _xdr_with_mem_ptr(xdr::field_destructor, type_, *this); }
    _pledges_t &operator=(const _pledges_t &source) {
      if (_xdr_field_number(type_)
          == _xdr_field_number(source.type_))
        _xdr_with_mem_ptr(xdr::field_assigner, type_, *this, source);
      else {
        this->~_pledges_t();
        type_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this, source);
      }
      type_ = source.type_;
      return *this;
    }
    _pledges_t &operator=(_pledges_t &&source) {
      if (_xdr_field_number(type_)
           == _xdr_field_number(source.type_))
        _xdr_with_mem_ptr(xdr::field_assigner, type_, *this,
                          std::move(source));
      else {
        this->~_pledges_t();
        type_ = std::numeric_limits<_xdr_case_type>::max();
        _xdr_with_mem_ptr(xdr::field_constructor, source.type_, *this,
                          std::move(source));
      }
      type_ = source.type_;
      return *this;
    }

    SCPStatementType type() const { return SCPStatementType(type_); }
    _pledges_t &type(SCPStatementType _xdr_d, bool _xdr_validate = true) {
      _xdr_discriminant(_xdr_d, _xdr_validate);
      return *this;
    }

    _prepare_t &prepare() {
      if (_xdr_field_number(type_) == 1)
        return prepare_;
      throw xdr::xdr_wrong_union("_pledges_t: prepare accessed when not selected");
    }
    const _prepare_t &prepare() const {
      if (_xdr_field_number(type_) == 1)
        return prepare_;
      throw xdr::xdr_wrong_union("_pledges_t: prepare accessed when not selected");
    }
    _confirm_t &confirm() {
      if (_xdr_field_number(type_) == 2)
        return confirm_;
      throw xdr::xdr_wrong_union("_pledges_t: confirm accessed when not selected");
    }
    const _confirm_t &confirm() const {
      if (_xdr_field_number(type_) == 2)
        return confirm_;
      throw xdr::xdr_wrong_union("_pledges_t: confirm accessed when not selected");
    }
    _externalize_t &externalize() {
      if (_xdr_field_number(type_) == 3)
        return externalize_;
      throw xdr::xdr_wrong_union("_pledges_t: externalize accessed when not selected");
    }
    const _externalize_t &externalize() const {
      if (_xdr_field_number(type_) == 3)
        return externalize_;
      throw xdr::xdr_wrong_union("_pledges_t: externalize accessed when not selected");
    }
    SCPNomination &nominate() {
      if (_xdr_field_number(type_) == 4)
        return nominate_;
      throw xdr::xdr_wrong_union("_pledges_t: nominate accessed when not selected");
    }
    const SCPNomination &nominate() const {
      if (_xdr_field_number(type_) == 4)
        return nominate_;
      throw xdr::xdr_wrong_union("_pledges_t: nominate accessed when not selected");
    }
  };

  NodeID nodeID{};
  uint64 slotIndex{};
  _pledges_t pledges{};

  SCPStatement() = default;
  template<typename _nodeID_T,
           typename _slotIndex_T,
           typename _pledges_T,
           typename = typename
           std::enable_if<std::is_constructible<NodeID, _nodeID_T>::value
                          && std::is_constructible<uint64, _slotIndex_T>::value
                          && std::is_constructible<_pledges_t, _pledges_T>::value
                         >::type>
  explicit SCPStatement(_nodeID_T &&_nodeID,
                        _slotIndex_T &&_slotIndex,
                        _pledges_T &&_pledges)
    : nodeID(std::forward<_nodeID_T>(_nodeID)),
      slotIndex(std::forward<_slotIndex_T>(_slotIndex)),
      pledges(std::forward<_pledges_T>(_pledges)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::SCPStatement::_pledges_t::_prepare_t>
  : xdr_struct_base<field_ptr<::vixal::SCPStatement::_pledges_t::_prepare_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_prepare_t::quorumSetHash),
                              &::vixal::SCPStatement::_pledges_t::_prepare_t::quorumSetHash>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_prepare_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_prepare_t::ballot),
                              &::vixal::SCPStatement::_pledges_t::_prepare_t::ballot>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_prepare_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_prepare_t::prepared),
                              &::vixal::SCPStatement::_pledges_t::_prepare_t::prepared>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_prepare_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_prepare_t::preparedPrime),
                              &::vixal::SCPStatement::_pledges_t::_prepare_t::preparedPrime>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_prepare_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_prepare_t::nC),
                              &::vixal::SCPStatement::_pledges_t::_prepare_t::nC>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_prepare_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_prepare_t::nH),
                              &::vixal::SCPStatement::_pledges_t::_prepare_t::nH>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPStatement::_pledges_t::_prepare_t &obj) {
    archive(ar, obj.quorumSetHash, "quorumSetHash");
    archive(ar, obj.ballot, "ballot");
    archive(ar, obj.prepared, "prepared");
    archive(ar, obj.preparedPrime, "preparedPrime");
    archive(ar, obj.nC, "nC");
    archive(ar, obj.nH, "nH");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPStatement::_pledges_t::_prepare_t &obj) {
    archive(ar, obj.quorumSetHash, "quorumSetHash");
    archive(ar, obj.ballot, "ballot");
    archive(ar, obj.prepared, "prepared");
    archive(ar, obj.preparedPrime, "preparedPrime");
    archive(ar, obj.nC, "nC");
    archive(ar, obj.nH, "nH");
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::SCPStatement::_pledges_t::_confirm_t>
  : xdr_struct_base<field_ptr<::vixal::SCPStatement::_pledges_t::_confirm_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_confirm_t::ballot),
                              &::vixal::SCPStatement::_pledges_t::_confirm_t::ballot>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_confirm_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_confirm_t::nPrepared),
                              &::vixal::SCPStatement::_pledges_t::_confirm_t::nPrepared>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_confirm_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_confirm_t::nCommit),
                              &::vixal::SCPStatement::_pledges_t::_confirm_t::nCommit>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_confirm_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_confirm_t::nH),
                              &::vixal::SCPStatement::_pledges_t::_confirm_t::nH>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_confirm_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_confirm_t::quorumSetHash),
                              &::vixal::SCPStatement::_pledges_t::_confirm_t::quorumSetHash>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPStatement::_pledges_t::_confirm_t &obj) {
    archive(ar, obj.ballot, "ballot");
    archive(ar, obj.nPrepared, "nPrepared");
    archive(ar, obj.nCommit, "nCommit");
    archive(ar, obj.nH, "nH");
    archive(ar, obj.quorumSetHash, "quorumSetHash");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPStatement::_pledges_t::_confirm_t &obj) {
    archive(ar, obj.ballot, "ballot");
    archive(ar, obj.nPrepared, "nPrepared");
    archive(ar, obj.nCommit, "nCommit");
    archive(ar, obj.nH, "nH");
    archive(ar, obj.quorumSetHash, "quorumSetHash");
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::SCPStatement::_pledges_t::_externalize_t>
  : xdr_struct_base<field_ptr<::vixal::SCPStatement::_pledges_t::_externalize_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_externalize_t::commit),
                              &::vixal::SCPStatement::_pledges_t::_externalize_t::commit>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_externalize_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_externalize_t::nH),
                              &::vixal::SCPStatement::_pledges_t::_externalize_t::nH>,
                    field_ptr<::vixal::SCPStatement::_pledges_t::_externalize_t,
                              decltype(::vixal::SCPStatement::_pledges_t::_externalize_t::commitQuorumSetHash),
                              &::vixal::SCPStatement::_pledges_t::_externalize_t::commitQuorumSetHash>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPStatement::_pledges_t::_externalize_t &obj) {
    archive(ar, obj.commit, "commit");
    archive(ar, obj.nH, "nH");
    archive(ar, obj.commitQuorumSetHash, "commitQuorumSetHash");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPStatement::_pledges_t::_externalize_t &obj) {
    archive(ar, obj.commit, "commit");
    archive(ar, obj.nH, "nH");
    archive(ar, obj.commitQuorumSetHash, "commitQuorumSetHash");
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::SCPStatement::_pledges_t> : xdr_traits_base {
  static constexpr const bool is_class = true;
  static constexpr const bool is_union = true;
  static constexpr const bool has_fixed_size = false;

  using union_type = ::vixal::SCPStatement::_pledges_t;
  using case_type = ::vixal::SCPStatement::_pledges_t::_xdr_case_type;
  using discriminant_type = decltype(std::declval<union_type>().type());

  static const char *union_field_name(case_type which) {
    switch (union_type::_xdr_field_number(which)) {
    case 1:
      return "prepare";
    case 2:
      return "confirm";
    case 3:
      return "externalize";
    case 4:
      return "nominate";
    }
    return nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::vixal::SCPStatement::_pledges_t &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of type in _pledges_t");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPStatement::_pledges_t &obj) {
    xdr::archive(ar, obj.type(), "type");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of type in _pledges_t");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPStatement::_pledges_t &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "type");
    obj.type(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                          union_field_name(which));
    xdr::validate(obj);
  }
};
template<> struct xdr_traits<::vixal::SCPStatement>
  : xdr_struct_base<field_ptr<::vixal::SCPStatement,
                              decltype(::vixal::SCPStatement::nodeID),
                              &::vixal::SCPStatement::nodeID>,
                    field_ptr<::vixal::SCPStatement,
                              decltype(::vixal::SCPStatement::slotIndex),
                              &::vixal::SCPStatement::slotIndex>,
                    field_ptr<::vixal::SCPStatement,
                              decltype(::vixal::SCPStatement::pledges),
                              &::vixal::SCPStatement::pledges>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPStatement &obj) {
    archive(ar, obj.nodeID, "nodeID");
    archive(ar, obj.slotIndex, "slotIndex");
    archive(ar, obj.pledges, "pledges");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPStatement &obj) {
    archive(ar, obj.nodeID, "nodeID");
    archive(ar, obj.slotIndex, "slotIndex");
    archive(ar, obj.pledges, "pledges");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct SCPEnvelope {
  SCPStatement statement{};
  Signature signature{};

  SCPEnvelope() = default;
  template<typename _statement_T,
           typename _signature_T,
           typename = typename
           std::enable_if<std::is_constructible<SCPStatement, _statement_T>::value
                          && std::is_constructible<Signature, _signature_T>::value
                         >::type>
  explicit SCPEnvelope(_statement_T &&_statement,
                       _signature_T &&_signature)
    : statement(std::forward<_statement_T>(_statement)),
      signature(std::forward<_signature_T>(_signature)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::SCPEnvelope>
  : xdr_struct_base<field_ptr<::vixal::SCPEnvelope,
                              decltype(::vixal::SCPEnvelope::statement),
                              &::vixal::SCPEnvelope::statement>,
                    field_ptr<::vixal::SCPEnvelope,
                              decltype(::vixal::SCPEnvelope::signature),
                              &::vixal::SCPEnvelope::signature>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPEnvelope &obj) {
    archive(ar, obj.statement, "statement");
    archive(ar, obj.signature, "signature");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPEnvelope &obj) {
    archive(ar, obj.statement, "statement");
    archive(ar, obj.signature, "signature");
    xdr::validate(obj);
  }
};
}
namespace vixal {

struct SCPQuorumSet {
  uint32 threshold{};
  xdr::xvector<PublicKey> validators{};
  xdr::xvector<SCPQuorumSet> innerSets{};

  SCPQuorumSet() = default;
  template<typename _threshold_T,
           typename _validators_T,
           typename _innerSets_T,
           typename = typename
           std::enable_if<std::is_constructible<uint32, _threshold_T>::value
                          && std::is_constructible<xdr::xvector<PublicKey>, _validators_T>::value
                          && std::is_constructible<xdr::xvector<SCPQuorumSet>, _innerSets_T>::value
                         >::type>
  explicit SCPQuorumSet(_threshold_T &&_threshold,
                        _validators_T &&_validators,
                        _innerSets_T &&_innerSets)
    : threshold(std::forward<_threshold_T>(_threshold)),
      validators(std::forward<_validators_T>(_validators)),
      innerSets(std::forward<_innerSets_T>(_innerSets)) {}
};
} 
namespace xdr {
template<> struct xdr_traits<::vixal::SCPQuorumSet>
  : xdr_struct_base<field_ptr<::vixal::SCPQuorumSet,
                              decltype(::vixal::SCPQuorumSet::threshold),
                              &::vixal::SCPQuorumSet::threshold>,
                    field_ptr<::vixal::SCPQuorumSet,
                              decltype(::vixal::SCPQuorumSet::validators),
                              &::vixal::SCPQuorumSet::validators>,
                    field_ptr<::vixal::SCPQuorumSet,
                              decltype(::vixal::SCPQuorumSet::innerSets),
                              &::vixal::SCPQuorumSet::innerSets>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vixal::SCPQuorumSet &obj) {
    archive(ar, obj.threshold, "threshold");
    archive(ar, obj.validators, "validators");
    archive(ar, obj.innerSets, "innerSets");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vixal::SCPQuorumSet &obj) {
    archive(ar, obj.threshold, "threshold");
    archive(ar, obj.validators, "validators");
    archive(ar, obj.innerSets, "innerSets");
    xdr::validate(obj);
  }
};
}
namespace vixal {

}

#endif // !__XDR__USERS_LIUWEI_WORKS_BLOCKCHAIN_SRC_GITHUB_COM_VIXLEDGER_VIXAL_CORE_INCLUDE_XDR_SCP_H_INCLUDED__
