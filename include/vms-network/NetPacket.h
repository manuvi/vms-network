/*
 * Copyright (C) 2026 Manuel Virgilio <real_virgil@yahoo.it>
 *
 * This file is part of vms-network.
 * Licensed under the GNU Lesser General Public License v2.1.
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace vms::network
{
    /**
     * @brief Mutable byte container for network payloads.
     *
     * Keeps both allocated buffer size and logical used byte count,
     * so callers can preallocate while still tracking valid payload length.
     */
    class NetPacket
    {
    public:
        NetPacket();

        /// Pointer to first byte of internal storage (const view).
        const uint8_t* data() const noexcept;
        /// Pointer to first byte of internal storage (mutable view).
        uint8_t* data() noexcept;

        /// Allocated buffer size in bytes.
        std::size_t size() const noexcept;
        /// Resizes allocated buffer. May clamp `used()` if shrinking.
        void resize(std::size_t new_size);
        /// Sets logical payload size, clamped to current `size()`.
        void set_used(std::size_t used_bytes);
        /// Logical payload size in bytes.
        std::size_t used() const noexcept;

        /// Direct access to underlying storage.
        std::vector<uint8_t>& buffer() noexcept;
        /// Read-only access to underlying storage.
        const std::vector<uint8_t>& buffer() const noexcept;

        /**
         * @brief Writes `count` bytes from `data` at `offset`.
         * @throws std::out_of_range if `count > data.size()`.
         * @throws std::overflow_error on `offset + count` overflow.
         */
        void write(std::size_t offset, const std::vector<uint8_t>& data, std::size_t count);
        /// Clears logical payload size (`used() = 0`) and keeps allocation.
        void reset() noexcept;

    private:
        std::vector<uint8_t> buffer_;
        std::size_t used_;
    };

    using NetPacketPtr = std::shared_ptr<NetPacket>;
}
