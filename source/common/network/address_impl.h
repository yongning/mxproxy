#pragma once

#include <sys/types.h>

#include <array>
#include <cstring>
#include <string>

#include "envoy/common/platform.h"
#include "envoy/network/address.h"

namespace Envoy {
namespace Network {
namespace Address {

InstanceConstSharedPtr addressFromSockAddr(const sockaddr_storage& ss, socklen_t len,
                                           bool v6only = true);

class InstanceBase : public Instance {
public:
    const std::string& asString() const override { return friendly_name_; }
    absl::string_view asStringView() const override { return friendly_name_; }
    const std::string& logicalName() const override { return asString(); }
    Type type() const override { return type_; }

protected:
    InstanceBase(Type type) : type_(type) {}
    std::string friendly_name_;

private:
    const Type type_;
};

class Ipv4Instance : public InstanceBase {
public:
    explicit Ipv4Instance(const sockaddr_in* address);
    explicit Ipv4Instance(const std::string& address);
    Ipv4Instance(const std::string& address, uint32_t port);
    explicit Ipv4Instance(uint32_t port);

    bool operator==(const Instance& rhs) const override;
    const Ip* ip() const override { return &ip_; }
    const Pipe* pipe() const override { return nullptr; }
    const sockaddr* sockAddr() const override {
        return reinterpret_cast<const sockaddr*>(&ip_.ipv4_.address_);
    }

    socklen_t sockAddrlen() const override { return sizeof(sockaddr_in); }

    static std::string sockaddrToString(const sockaddr_in& addr);

private:
    struct Ipv4Helper : public Ipv4 {
        uint32_t address() const override { return address_.sin_addr.s_addr; }

        sockaddr_in address_;
    };

    struct IpHelper : public Ip {
        const std::string& addressAsString() const override { return friendly_address_; }
        bool isAnyAddress() const override { return ipv4_.address_.sin_addr.s_addr == INADDR_ANY; }
        bool isUnicastAddress() const override {
            return !isAnyAddress() && (ipv4_.address_.sin_addr.s_addr != INADDR_BROADCAST) && 
                   !((ipv4_.address_.sin_addr.s_addr & htonl(0xf0000000)) == htonl(0xe0000000));
        }
        const Ipv4* ipv4() const override { return &ipv4_; }
        const Ipv6* ipv6() const override { return nullptr; }
        uint32_t port() const override { return ntohs(ipv4_.address_.sin_port); }
        IpVersion version() const override { return IpVersion::v4 }
        
        Ipv4Helper ipv4_;
        std::string friendly_address_;
    };

    IpHelper ip_;
};


    
