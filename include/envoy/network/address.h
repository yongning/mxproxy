#pragma once

#include <sys/types.h>

#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include "envoy/api/os_sys_calls.h"
#include "envoy/common/platform.h"
#include "envoy/common/pure.h"

#include "absl/numeric/int128.h"
#include "absl/strings/string_view.h"

namespace Envoy {
namespace Network {
namespace Address {

class Ipv4 {
public:
    virtual ~Ipv4() = default;
    virtual uint32_t address() const PURE;
};

class Ipv6 {
public:
    virtual ~Ipv6() = default;
    virtual absl::uint128 address() const PURE;
    virtual bool v6only() const PURE;
};

enum class IpVersion { v4, v6 };

class Ip {
public:
    virtual ~Ip() = default;

    virtual const std::string& addressAsString() const PURE;
    virtual bool isAnyAddress() const PURE;

    virtual bool isUnicastAddress() const PURE;

    virtual const Ipv4* ipv4() const PURE;

    virtual const Ipv6* ipv6() const PURE;

    virtual uint32_t port() const PURE;

    virtual IpVersion version() const PURE;
};

class Pipe {
public:
    virtual ~Pipe() = default;
    virtual bool abstractNamespace() const PURE;
    virtual mode_t mode() const PURE;
};

enum class Type { Ip, Pipe };

class Instance {
public:
    virtual ~Instance() = default;

    virtual bool operator==(const Instance& rhs) const PURE;
    bool operator!=(const Instance& rhs) const { return !operator==(rhs); }

    virtual const std::string& asString() const PURE;

    virtual absl::string_view asStringView() const PURE;

    virtual const std::string& logicalName() const PURE;

    virtual const Ip* ip() const PURE;

    virtual const Pipe* pipe() const PURE;

    virtual const sockaddr* sockAddr() const PURE;

    virtual socklen_t sockAddrLen() const PURE;

    virtual Type type() const PURE;
};

using InstanceConstSharedPtr = std::shared_ptr<const Instance>;

}
}
}


