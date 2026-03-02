#include <vms-network/NetAddress.h>
#include <vms-network/NetSocket.h>

#include <fcntl.h>
#include <unistd.h>

#include <cassert>
#include <cstdint>

using vms::network::NetAddress;
using vms::network::NetSocket;
using vms::network::NetSocketType;

int main()
{
    {
        NetSocket socket(NetSocketType::UDP);
        assert(socket.fd() == -1);
        assert(socket.error() == 0);
    }

    {
        NetSocket socket(NetSocketType::UDP);
        NetAddress bind_address;
        bind_address.set("", 0U);

        assert(!socket.bind(bind_address));
        assert(socket.error() != 0);
    }

    {
        const int duplicated_fd = dup(STDIN_FILENO);
        assert(duplicated_fd >= 0);

        NetSocket socket(NetSocketType::UDP, duplicated_fd);

        assert(socket.set_non_blocking());
        assert(socket.error() == 0);

        const int flags = fcntl(socket.fd(), F_GETFL, 0);
        assert(flags >= 0);
        assert((flags & O_NONBLOCK) != 0);

        socket.close();
        assert(socket.fd() == -1);
    }

    {
        const int duplicated_fd = dup(STDIN_FILENO);
        assert(duplicated_fd >= 0);

        NetSocket original(NetSocketType::UDP, duplicated_fd);
        const int original_fd = duplicated_fd;
        assert(original_fd >= 0);

        NetSocket moved(std::move(original));
        assert(original.fd() == -1);
        assert(moved.fd() == original_fd);
    }

    {
        const int duplicated_fd = dup(STDIN_FILENO);
        assert(duplicated_fd >= 0);

        NetSocket source(NetSocketType::UDP, duplicated_fd);
        NetSocket destination(NetSocketType::UDP);
        const int source_fd = duplicated_fd;
        assert(source_fd >= 0);

        destination = std::move(source);
        assert(source.fd() == -1);
        assert(destination.fd() == source_fd);
    }

    {
        NetSocket socket(NetSocketType::UDP);
        const bool init_result = socket.init();

        if (!init_result) {
            assert(socket.error() != 0);
        } else {
            assert(socket.fd() >= 0);
            assert(socket.error() == 0);
            socket.close();
        }
    }

    return 0;
}
