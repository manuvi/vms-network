#include <vms-network/NetAddress.h>
#include <vms-network/NetPacket.h>
#include <vms-network/TCPSocket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

using vms::network::NetAddress;
using vms::network::NetPacket;
using vms::network::TcpSocket;

int main()
{
    {
        TcpSocket socket;
        assert(socket.fd() == -1);
        assert(socket.last_error() == 0);
    }

    {
        TcpSocket socket;
        NetAddress endpoint;
        endpoint.set("127.0.0.1", 12345U);

        assert(!socket.connect(endpoint));
        assert(socket.last_error() != 0);
    }

    {
        TcpSocket socket;
        NetPacket packet;
        const std::vector<std::uint8_t> payload{1U, 2U, 3U};
        packet.write(0U, payload, payload.size());

        assert(!socket.send(packet));
        assert(socket.last_error() != 0);
    }

    {
        TcpSocket socket;
        NetPacket packet;

        assert(!socket.receive(packet));
        assert(socket.last_error() != 0);
    }

    {
        TcpSocket socket;
        const bool init_result = socket.init();

        if (!init_result) {
            assert(socket.error() != 0);
        } else {
            assert(socket.fd() >= 0);
            socket.close();
            assert(socket.fd() == -1);
        }
    }

    {
        const int raw_socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (raw_socket >= 0) {
            TcpSocket socket(raw_socket);
            const int original_fd = socket.fd();
            assert(original_fd >= 0);

            TcpSocket moved(std::move(socket));
            assert(socket.fd() == -1);
            assert(moved.fd() == original_fd);

            TcpSocket destination;
            destination = std::move(moved);
            assert(moved.fd() == -1);
            assert(destination.fd() == original_fd);
        }
    }

    {
        const int listener_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (listener_fd >= 0) {
            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            server_addr.sin_port = 0;

            const bool bind_ok = (::bind(
                listener_fd,
                reinterpret_cast<const sockaddr*>(&server_addr),
                sizeof(server_addr)
            ) == 0);
            const bool listen_ok = bind_ok && (::listen(listener_fd, 1) == 0);

            if (listen_ok) {
                sockaddr_in bound_addr{};
                socklen_t bound_len = sizeof(bound_addr);
                if (::getsockname(listener_fd, reinterpret_cast<sockaddr*>(&bound_addr), &bound_len) == 0) {
                    TcpSocket client;
                    if (client.init()) {
                        NetAddress endpoint;
                        endpoint.set("127.0.0.1", ntohs(bound_addr.sin_port));

                        if (client.connect(endpoint)) {
                            client.set_mtu(2048U);
                            const int accepted_fd = ::accept(listener_fd, nullptr, nullptr);
                            if (accepted_fd >= 0) {
                                NetPacket outgoing;
                                const std::vector<std::uint8_t> payload{10U, 20U, 30U, 40U};
                                outgoing.write(0U, payload, payload.size());

                                assert(client.send(outgoing));

                                std::vector<std::uint8_t> received(payload.size(), 0U);
                                const ssize_t server_read = ::recv(
                                    accepted_fd,
                                    received.data(),
                                    received.size(),
                                    0
                                );
                                assert(server_read == static_cast<ssize_t>(payload.size()));
                                for (std::size_t i = 0; i < payload.size(); ++i) {
                                    assert(received[i] == payload[i]);
                                }

                                const std::vector<std::uint8_t> reply{7U, 8U, 9U};
                                const ssize_t server_sent = ::send(
                                    accepted_fd,
                                    reply.data(),
                                    reply.size(),
                                    0
                                );
                                assert(server_sent == static_cast<ssize_t>(reply.size()));

                                client.set_mtu(1U);
                                NetPacket incoming;
                                assert(client.receive(incoming));
                                assert(incoming.used() == 1U);
                                assert(incoming.buffer()[0] == reply[0]);

                                ::close(accepted_fd);
                            }
                        }
                    }
                }
            }

            ::close(listener_fd);
        }
    }

    return 0;
}
