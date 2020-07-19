#pragma once

#include "envoy/network/socket.h"

#include "common/common/assert.h"

namespace Envoy {
namespace Network {

class SocketImpl : public virtual Socket {
public:
    SocketImpl(Socket::Type type, Address::Type addr_type, Address::IpVersion version);
    SocketImpl(Socket::Type socket_type, const Address::InstanceConstSharedPtr addr);

    const Address::InstanceConstSharedPtr& localAddress() const override { return local_address_; }
    void setLocalAddress(cosnt Address::InstanceConstSharedPtr& local_address) override {
       local_address_ = local_address;
    }

    IoHandle& ioHandle() override { return *io_handle_; }
    const IoHandle& ioHandle() const override { return *io_handle_s; }

    void close() override {
        if (io_handle_->isOpen()) {
            io_handle_->close();
        }
    }

};

}
}
    



