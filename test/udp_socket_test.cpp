#include <vms-network/NetAddress.h>
#include <vms-network/NetPacket.h>
#include <vms-network/UDPSocket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cassert>
#include <cstdint>
#include <vector>

using vms::network::NetAddress;
using vms::network::NetPacket;
using vms::network::UdpSocket;

int main()
{
    {
        UdpSocket socket;
        assert(socket.last_error() == 0);
    }

    {
        UdpSocket socket;
        NetAddress destination;
        destination.set("127.0.0.1", 12345U);

        NetPacket packet;
        const std::vector<std::uint8_t> payload{1U, 2U, 3U};
        packet.write(0U, payload, payload.size());

        assert(!socket.send(packet, destination));
        assert(socket.last_error() != 0);
    }

    {
        NetPacket packet;
        NetAddress from;
        UdpSocket socket(2048U);

        assert(!socket.receive(packet, from));
        assert(socket.last_error() != 0);
        assert(packet.size() == 2048U);
    }

    {
        UdpSocket receiver;
        UdpSocket sender;

        if (receiver.init() && sender.init()) {
            NetAddress bind_address;
            bind_address.set("127.0.0.1", 0U);

            if (receiver.bind(bind_address)) {
                sockaddr_in local_addr{};
                socklen_t local_addr_size = sizeof(local_addr);
                const int getsockname_result = getsockname(
                    receiver.fd(),
                    reinterpret_cast<sockaddr*>(&local_addr),
                    &local_addr_size
                );

                if (getsockname_result == 0) {
                    NetAddress destination;
                    destination.set("127.0.0.1", ntohs(local_addr.sin_port));

                    NetPacket outgoing;
                    const std::vector<std::uint8_t> payload{10U, 20U, 30U, 40U};
                    outgoing.write(0U, payload, payload.size());

                    assert(sender.send(outgoing, destination));
                    assert(sender.last_error() == 0);

                    NetPacket incoming;
                    NetAddress from;
                    assert(receiver.receive(incoming, from));
                    assert(receiver.last_error() == 0);

                    assert(incoming.used() == payload.size());
                    for (std::size_t i = 0; i < payload.size(); ++i) {
                        assert(incoming.buffer()[i] == payload[i]);
                    }
                }
            }
        }
    }

    return 0;
}
