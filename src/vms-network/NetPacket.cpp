/*
 * Copyright (C) 2026 Manuel Virgilio <real_virgil@yahoo.it>
 *
 * This file is part of vms-network.
 * Licensed under the GNU Lesser General Public License v2.1.
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#include <vms-network/NetPacket.h>

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace vms::network
{
    NetPacket::NetPacket()
        : used_(0)
    {
    }

    const uint8_t* NetPacket::data() const noexcept
    {
        return buffer_.data();
    }

    uint8_t* NetPacket::data() noexcept
    {
        return buffer_.data();
    }

    std::size_t NetPacket::size() const noexcept
    {
        return buffer_.size();
    }

    void NetPacket::resize(std::size_t new_size)
    {
        buffer_.resize(new_size);
        if (used_ > new_size) {
            used_ = new_size;
        }
    }

    void NetPacket::set_used(std::size_t used_bytes)
    {
        used_ = std::min(used_bytes, buffer_.size());
    }

    std::size_t NetPacket::used() const noexcept
    {
        return used_;
    }

    std::vector<uint8_t>& NetPacket::buffer() noexcept
    {
        return buffer_;
    }

    const std::vector<uint8_t>& NetPacket::buffer() const noexcept
    {
        return buffer_;
    }

    void NetPacket::write(std::size_t offset, const std::vector<uint8_t>& data, std::size_t count)
    {
        if (count > data.size()) {
            throw std::out_of_range("NetPacket::write count exceeds input data size");
        }
        if (offset > std::numeric_limits<std::size_t>::max() - count) {
            throw std::overflow_error("NetPacket::write offset + count overflow");
        }

        const std::size_t required_size = offset + count;
        if (buffer_.size() < required_size) {
            buffer_.resize(required_size);
        }

        std::copy_n(data.begin(), count, buffer_.begin() + offset);
        used_ = std::max(used_, required_size);
    }

    void NetPacket::reset() noexcept
    {
        used_ = 0;
    }
}
