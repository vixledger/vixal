//
// Copyright (c) 2012 Daniel Lundin
//

#include "medida/reporting/collectd_reporter.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mutex>
#include <sstream>
#include <stdexcept>

#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>

#include "medida/metrics_registry.h"
#include "medida/reporting/util.h"

#ifdef __APPLE__

#include <libkern/OSByteOrder.h>

#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)
#endif

namespace medida {
namespace reporting {

class CollectdReporter::Impl {
public:
    Impl(CollectdReporter &self, MetricsRegistry &registry, const std::string &hostname = "127.0.0.1",
         std::uint16_t port = 25826);

    ~Impl();

    void run();

    void process(Counter &counter);

    void process(Meter &meter);

    void process(Histogram &histogram);

    void process(Timer &timer);

private:
    enum PartType {
        kHost = 0x0000,
        kTime = 0x0001,
        kPlugin = 0x0002,
        kPluginInstance = 0x0003,
        kType = 0x0004,
        kTypeInstance = 0x0005,
        kValues = 0x0006,
        kInterval = 0x0007,
        kMessage = 0x0100,
        kSeverity = 0x0101
    };
    enum DataType {
        kCounter = 0x00,
        kGauge = 0x01,
        kDerive = 0x02,
        kAbsolute = 0x03
    };
    struct Value {
        DataType type;
        double value;
    };
    static const int kMaxSize = 1024;
    CollectdReporter &self_;
    MetricsRegistry &registry_;
    std::string uname_;
    std::mutex mutex_;
    struct addrinfo *addrinfo_;
    int socket_;
    char msgbuf_[kMaxSize];
    char *msgbuf_ptr_;
    std::string current_instance_;

    void addPart(PartType type, std::uint64_t number);

    void addPart(PartType type, const std::string &text);

    void addValues(std::initializer_list<Value> values);

    inline void pack8(std::uint8_t data);

    inline void pack16(std::uint16_t data);

    inline void pack64(std::uint64_t data);

    inline void pack_double(double data);

    inline void move_ptr(std::uint16_t offset);
};


CollectdReporter::CollectdReporter(MetricsRegistry &registry, const std::string &hostname, std::uint16_t port)
        : AbstractPollingReporter(),
          impl_(std::make_unique<CollectdReporter::Impl>(*this, registry, hostname, port)) {
}


CollectdReporter::~CollectdReporter() = default;


void CollectdReporter::run() {
    impl_->run();
}


void CollectdReporter::process(Counter &counter) {
    impl_->process(counter);
}


void CollectdReporter::process(Meter &meter) {
    impl_->process(meter);
}


void CollectdReporter::process(Histogram &histogram) {
    impl_->process(histogram);
}


void CollectdReporter::process(Timer &timer) {
    impl_->process(timer);
}


// === Implementation ===


CollectdReporter::Impl::Impl(CollectdReporter &self, MetricsRegistry &registry, const std::string &hostname,
                             std::uint16_t port)
        : self_(self),
          registry_(registry) {
    utsname name;
    uname_ = uname(&name) ? "localhost" : name.nodename;
    auto port_str = std::to_string(port);
    auto err = getaddrinfo(hostname.c_str(), port_str.c_str(), nullptr, &addrinfo_);
    if (err != 0) {
        throw std::invalid_argument("getaddrinfo error: " + std::string(gai_strerror(err)));
    }
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ == -1) {
        std::stringstream ss;
        ss << "Socket error (" << errno << "): " << strerror(errno);
        throw std::runtime_error(ss.str());
    }
}


CollectdReporter::Impl::~Impl() {
    freeaddrinfo(addrinfo_);
}


void CollectdReporter::Impl::run() {
    std::lock_guard<std::mutex> lock{mutex_};
    for (auto &kv : registry_.getAllMetrics()) {
        auto name = kv.first;
        auto metric = kv.second;
        auto scope = name.scope();
        current_instance_ = name.name() + (scope.empty() ? "" : "." + scope);

        // reset message
        msgbuf_ptr_ = &msgbuf_[0];

        // Add message parts
        addPart(kTime, static_cast<uint64_t>(std::time(nullptr)));
        addPart(kHost, uname_);
        addPart(kPlugin, name.domain() + "." + name.type());
        metric->process(self_);

        // Send message
        auto msg_size = msgbuf_ptr_ - msgbuf_;
        sendto(socket_, msgbuf_, msg_size, 0, addrinfo_->ai_addr, addrinfo_->ai_addrlen);
    }
}


void CollectdReporter::Impl::process(Counter &counter) {
    double count = counter.count();
    addPart(kType, "medida_counter");
    addPart(kTypeInstance, current_instance_ + ".count");
    addValues({{kGauge, count}});
}


void CollectdReporter::Impl::process(Meter &meter) {
    auto event_type = meter.event_type();
    auto unit = FormatRateUnit(meter.rate_unit());
    double count = meter.count();
    addPart(kType, "medida_meter");
    addPart(kTypeInstance, current_instance_ + "." + event_type + "_per_" + unit);
    addValues({
                      {kGauge, count},
                      {kGauge, meter.mean_rate()},
                      {kGauge, meter.one_minute_rate()},
                      {kGauge, meter.five_minute_rate()},
                      {kGauge, meter.fifteen_minute_rate()},
              });
}


void CollectdReporter::Impl::process(Histogram &histogram) {
    auto snapshot = histogram.snapshot();
    addPart(kType, "medida_histogram");
    addPart(kTypeInstance, current_instance_);
    addValues({
                      {kGauge, histogram.min()},
                      {kGauge, histogram.max()},
                      {kGauge, histogram.mean()},
                      {kGauge, histogram.std_dev()},
                      {kGauge, snapshot.getMedian()},
                      {kGauge, snapshot.get75thPercentile()},
                      {kGauge, snapshot.get95thPercentile()},
                      {kGauge, snapshot.get98thPercentile()},
                      {kGauge, snapshot.get99thPercentile()},
                      {kGauge, snapshot.get999thPercentile()},
              });
}


void CollectdReporter::Impl::process(Timer &timer) {
    auto snapshot = timer.snapshot();
    addPart(kType, "medida_timer");
    addPart(kTypeInstance, current_instance_ + "." + FormatRateUnit(timer.duration_unit()));
    addValues({
                      {kGauge, timer.min()},
                      {kGauge, timer.max()},
                      {kGauge, timer.mean()},
                      {kGauge, timer.std_dev()},
                      {kGauge, snapshot.getMedian()},
                      {kGauge, snapshot.get75thPercentile()},
                      {kGauge, snapshot.get95thPercentile()},
                      {kGauge, snapshot.get98thPercentile()},
                      {kGauge, snapshot.get99thPercentile()},
                      {kGauge, snapshot.get999thPercentile()},
              });
}


void CollectdReporter::Impl::addPart(PartType type, std::uint64_t number) {
    pack16(type);
    pack16(12);
    pack64(number);
}


void CollectdReporter::Impl::addPart(PartType type, const std::string &text) {
    auto len = text.size() + 1;
    pack16(type);
    pack16(len + 4);
    memcpy(msgbuf_ptr_, text.c_str(), len);
    move_ptr(len);
}


void CollectdReporter::Impl::addValues(std::initializer_list<Value> values) {
    auto count = values.size();
    pack16(PartType::kValues);
    pack16(6 + count * 9); // 48 bit header, 8 + 64 bits per value
    pack16(count);
    for (auto &v : values) {
        pack8(v.type);
    }
    for (auto &v : values) {
        if (v.type == DataType::kGauge) {
            pack_double(v.value);
        } else {
            pack64(v.value);
        }
    }
}


void CollectdReporter::Impl::pack8(std::uint8_t data) {
    *msgbuf_ptr_ = data;
    move_ptr(1);
}


void CollectdReporter::Impl::pack16(std::uint16_t data) {
    *(std::uint16_t *) msgbuf_ptr_ = htobe16(data);
    move_ptr(2);
}


void CollectdReporter::Impl::pack64(std::uint64_t data) {
    *(std::uint64_t *) msgbuf_ptr_ = htobe64(data);
    move_ptr(8);
}


void CollectdReporter::Impl::pack_double(double data) {
    // FIXME: Should break on bigendian archs. collectd expects little-endian doubles
    *(double *) msgbuf_ptr_ = data;
    move_ptr(8);
}


void CollectdReporter::Impl::move_ptr(std::uint16_t offset) {
    msgbuf_ptr_ += offset;
    if (msgbuf_ptr_ > msgbuf_ + kMaxSize) {
        throw std::runtime_error("Message buffer overflow");
    }
}


} // namespace reporting
} // namespace medida
