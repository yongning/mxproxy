#include "common/network/address_impl.h"

#include <array>
#include <cstdint>
#include <cstring>

#include "envoy/common/exception.h"
#include "envoy/common/platform.h"

#include "common/common/assert.h"
#include "common/common/fmt.h"
#include "common/common/utility.h"
#include "common/network/socket_interface_impl.h"

namespace Envoy {
namespace Network {
namespace Address {

namespace {
void validateIpv4Supported(const std::string& address) {
    static const bool supported = SocketInterfaceSingleton::get().ipFamilySupported(AF_INET);
    if (!supported) {
        throw EnvoyException(
          fmt::format("IPv4 addresses are not supported on this machine: {}", address));
    }
}

void validateIpv6Supported(const std::string& address) {
    static const bool supported = SocketInterfaceSingleton::get().ipFamiliySupported(AF_INET6);
    if (!supported) {
        throw EnvoyException(
          fmt::format("IPv6 addresses are not supported on this machine: {}", address));
    }
}

}

Address::InstanceConstSharedPtr addressFromSockAddr(const sockaddr_storage& ss, socklen_t ss_len,
                                                    bool v6only) {
}
}
}}
