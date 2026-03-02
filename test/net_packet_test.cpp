#include <vms-network/NetPacket.h>

#include <cassert>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

using vms::network::NetPacket;

int main()
{
    {
        NetPacket packet;
        assert(packet.size() == 0U);
        assert(packet.used() == 0U);
    }

    {
        NetPacket packet;
        packet.resize(4U);

        uint8_t* raw = packet.data();
        assert(raw != nullptr);

        raw[0] = 9U;
        raw[1] = 8U;
        raw[2] = 7U;
        raw[3] = 6U;

        assert(packet.buffer()[0] == 9U);
        assert(packet.buffer()[1] == 8U);
        assert(packet.buffer()[2] == 7U);
        assert(packet.buffer()[3] == 6U);
    }

    {
        NetPacket packet;
        const std::vector<uint8_t> input{5U, 6U, 7U};
        packet.write(1U, input, input.size());

        const NetPacket& const_packet = packet;
        const uint8_t* const_raw = const_packet.data();
        const std::vector<uint8_t>& const_buffer = const_packet.buffer();

        assert(const_raw != nullptr);
        assert(const_packet.size() == const_buffer.size());
        assert(const_raw[0] == const_buffer[0]);
        assert(const_raw[1] == 5U);
        assert(const_raw[3] == 7U);
    }

    {
        NetPacket packet;
        packet.resize(8U);
        assert(packet.size() == 8U);

        packet.set_used(6U);
        assert(packet.used() == 6U);

        packet.set_used(20U);
        assert(packet.used() == 8U);
    }

    {
        NetPacket packet;
        packet.resize(10U);
        packet.set_used(9U);
        packet.resize(4U);

        assert(packet.size() == 4U);
        assert(packet.used() == 4U);
    }

    {
        NetPacket packet;
        const std::vector<uint8_t> input{1U, 2U, 3U, 4U};
        packet.write(2U, input, input.size());

        assert(packet.size() == 6U);
        assert(packet.used() == 6U);
        assert(packet.buffer()[0] == 0U);
        assert(packet.buffer()[1] == 0U);
        assert(packet.buffer()[2] == 1U);
        assert(packet.buffer()[5] == 4U);
    }

    {
        NetPacket packet;
        const std::vector<uint8_t> input{10U, 11U, 12U, 13U};
        packet.write(4U, input, input.size());
        assert(packet.used() == 8U);

        packet.write(0U, input, 2U);
        assert(packet.used() == 8U);
        assert(packet.buffer()[0] == 10U);
        assert(packet.buffer()[1] == 11U);
    }

    {
        NetPacket packet;
        packet.resize(16U);
        packet.set_used(10U);
        packet.reset();

        assert(packet.used() == 0U);
        assert(packet.size() == 16U);
    }

    {
        NetPacket packet;
        const std::vector<uint8_t> input{1U, 2U};
        bool thrown = false;

        try {
            packet.write(0U, input, 3U);
        } catch (const std::out_of_range&) {
            thrown = true;
        }

        assert(thrown);
    }

    {
        NetPacket packet;
        const std::vector<uint8_t> input{42U};
        bool thrown = false;

        try {
            packet.write(std::numeric_limits<std::size_t>::max(), input, 1U);
        } catch (const std::overflow_error&) {
            thrown = true;
        }

        assert(thrown);
    }

    return 0;
}
