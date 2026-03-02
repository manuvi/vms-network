#include <vms-network/NetAddress.h>

#include <arpa/inet.h>

#include <cassert>
#include <cstdint>
#include <string>

using vms::network::NetAddress;
using vms::network::NetAddressHash;

int main()
{
    {
        NetAddress address;
        assert(address.is_valid());
        assert(address.error() == 0);
        assert(address.size() == sizeof(sockaddr_in));
    }

    {
        NetAddress address;
        address.set("", 4200U);

        assert(address.is_valid());

        std::string ip;
        std::uint16_t port = 0;
        address.get(ip, port);

        assert(ip == "0.0.0.0");
        assert(port == 4200U);
    }

    {
        NetAddress address;
        address.set("127.0.0.1", 12345U);

        assert(address.is_valid());

        std::string ip;
        std::uint16_t port = 0;
        address.get(ip, port);

        assert(ip == "127.0.0.1");
        assert(port == 12345U);
    }

    {
        NetAddress address;
        address.set("invalid-ip", 80U);

        assert(!address.is_valid());
        assert(address.error() != 0);
    }

    {
        NetAddress address;
        address.set("127.0.0.1", 80U);
        address.reset();

        assert(address.is_valid());
        assert(address.error() == 0);

        std::string ip;
        std::uint16_t port = 777U;
        address.get(ip, port);

        assert(ip == "0.0.0.0");
        assert(port == 0U);
    }

    {
        NetAddress a;
        NetAddress b;
        NetAddress c;

        a.set("127.0.0.1", 5000U);
        b.set("127.0.0.1", 5000U);
        c.set("127.0.0.1", 5001U);

        assert(a == b);
        assert(!(a == c));
    }

    {
        NetAddress address;
        address.set("127.0.0.1", 5002U);

        sockaddr* raw = address.sockaddr_ptr();
        assert(raw != nullptr);

        const NetAddress& const_address = address;
        const sockaddr* const_raw = const_address.sockaddr_ptr();
        assert(const_raw != nullptr);
    }

    {
        NetAddress a;
        NetAddress b;
        NetAddress c;

        a.set("127.0.0.1", 9000U);
        b.set("127.0.0.1", 9000U);
        c.set("127.0.0.2", 9000U);

        const NetAddressHash hash_fn;
        const std::size_t ha = hash_fn(a);
        const std::size_t hb = hash_fn(b);
        const std::size_t hc = hash_fn(c);

        assert(ha == hb);
        assert(ha != hc);
    }

    return 0;
}
