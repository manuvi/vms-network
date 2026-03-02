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
#include <functional>
#include <memory>
#include <string>

#include <netinet/in.h>

namespace vms::network
{
    /**
     * @brief IPv4 endpoint wrapper used by socket APIs.
     *
     * Stores address/port as `sockaddr_in` and exposes conversion helpers
     * for direct integration with POSIX socket calls.
     */
    class NetAddress
    {
    public:
        /// Builds `0.0.0.0:0` with a valid internal state.
        NetAddress() noexcept;

        /// Returns a read-only pointer to the internal sockaddr structure.
        const struct sockaddr* sockaddr_ptr() const noexcept;
        /// Returns a writable pointer to the internal sockaddr structure.
        struct sockaddr* sockaddr_ptr() noexcept;

        /// Size in bytes of the internal sockaddr structure.
        std::size_t size() const noexcept;
        /// Sets endpoint from dotted IPv4 string and port.
        void set(const std::string& address, std::uint16_t port) noexcept;
        /// Reads endpoint as dotted IPv4 string and host-order port.
        void get(std::string& address, std::uint16_t& port) const noexcept;

        /// True when last operation completed without parse/system errors.
        bool is_valid() const noexcept;
        /// Last error code associated with `set`/socket-compatible operations.
        int error() const noexcept;

        bool operator==(const NetAddress& other) const noexcept;

        /// Resets to `0.0.0.0:0` and clears error state.
        void reset() noexcept;

    private:
        struct sockaddr_in addr_;
        int error_;

        friend class NetAddressHash;
    };

    using NetAddressPtr = std::shared_ptr<NetAddress>;

    /// Hash functor suitable for unordered containers keyed by `NetAddress`.
    class NetAddressHash
    {
    public:
        std::size_t operator()(const NetAddress& key) const noexcept
        {
            const std::size_t h1 = std::hash<std::uint32_t>{}(key.addr_.sin_addr.s_addr);
            const std::size_t h2 = std::hash<std::uint32_t>{}(key.addr_.sin_port);
            return h1 ^ (h2 << 1);
        }
    };
}
