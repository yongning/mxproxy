#pragma once

#include "envoy/network/listener.h"

#include "common/event/dispatcher_impl.h"
#include "common/event/libevent.h"
#include "common/network/listen_socket_impl.h"

#include "event2/event.h"

namespace Envoy {
namespace Network {

class BaseListenerImpl : public virtual Listener {
public:
    BaseListenerImpl(Event::DispatcherImpl& dispatcher, SocketSharePtr socket);

protected:
    Address::InstanceConstSharedPtr local_address_;
    Event::DispatcherImpl& dispatcher_;
    const SocketSharedPtr socket_;
};

}
}
