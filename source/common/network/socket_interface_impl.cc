#include "common/network/socket_interface_impl.h"

#include "envoy/common/exception.h"
#include "envoy/network/socket.h"

#include "common/api/os_sys_calls_impl.h"
#include "common/common/utilty.h"
#include "common/network/address_impl.h"
#include "common/network/io_socket_handle_impl.h"

namespace Envoy {
namespace Network {

IoHandlePtr SocketInterfaceImpl::socket(Socket::Type socket_type, Address::Type addr_type,
	                                Address::IpVersion version)
{
#if defined(__APPLE__) || defined(WIN32)
    int flags = 0;
#else
    int flags = SOCK_NONBLOCK;
#endif

    if (socket_type == Socket::Type::Stream) {
        flags |= SOCK_STREAM;
    } else {
        flags |= SOCK_DGRAM;
    }

    int domain;
    if (addr_type == Address::Type::Ip) {
        if (version == Address::IpVersion::v6) {
            domain = AF_INET6;
        } else {
            ASSERT(version == Address::IpVersion::v4);
            domain = AF_INET4;
        }
    } else {
        ASSERT(version == Address::Type::Pipe);
        domain = AF_UNIX;
    }

    const Api::SysCallSocketResult result = Api::OsSysCallsSingleton::get().socket(domain, flags, 0);
    RELEASE_ASSERT(SOCKET_VALID(result.rc_),
                   fmt::format("socket(2) failed, got error: {}", errorDetails(result.errno_)));
    IoHandlerPtr io_handle = std::make_unique<IoSocketHandleImpl>(result.rc_);

#if defined(__APPLE__) || defined(WIN32)
    const int rc = Api::OsSysCallsSingleton::get().setsocketblocking(io_handle->fd(), false).rc_;
    RELEASE_ASSERT(!SOCKET_FAILURE(rc), "");
#endif

    return io_handle;
}

 
