/*
 * Copyright (C) 2026 Manuel Virgilio <real_virgil@yahoo.it>
 *
 * This file is part of vms-network.
 * Licensed under the GNU Lesser General Public License v2.1.
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#include <vms-network/NetAddress.h>

#include <arpa/inet.h>

#include <cerrno>
#include <cstring>

namespace vms::network
{
    NetAddress::NetAddress() noexcept
        : error_(0)
    {
        reset();
    }

    const sockaddr* NetAddress::sockaddr_ptr() const noexcept
    {
        return reinterpret_cast<const sockaddr*>(&addr_);
    }

    sockaddr* NetAddress::sockaddr_ptr() noexcept
    {
        return reinterpret_cast<sockaddr*>(&addr_);
    }

    std::size_t NetAddress::size() const noexcept
    {
        return sizeof(sockaddr_in);
    }

    void NetAddress::set(const std::string& address, std::uint16_t port) noexcept
    {
        std::memset(&addr_, 0, sizeof(sockaddr_in));
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port);
        error_ = 0;

        if (address.empty()) {
            addr_.sin_addr.s_addr = INADDR_ANY;
            return;
        }

        const int result = inet_pton(AF_INET, address.c_str(), &addr_.sin_addr);
        if (result == 1) {
            return;
        }
        if (result == 0) {
            error_ = EINVAL;
            addr_.sin_addr.s_addr = INADDR_ANY;
            return;
        }

        error_ = errno;
        addr_.sin_addr.s_addr = INADDR_ANY;
    }

    void NetAddress::get(std::string& address, std::uint16_t& port) const noexcept
    {
        char ip_buffer[INET_ADDRSTRLEN] = {0};
        const char* result = inet_ntop(AF_INET, &addr_.sin_addr, ip_buffer, sizeof(ip_buffer));

        if (result == nullptr) {
            address.clear();
            port = 0;
            return;
        }

        address = ip_buffer;
        port = ntohs(addr_.sin_port);
    }

    bool NetAddress::is_valid() const noexcept
    {
        return error_ == 0;
    }

    int NetAddress::error() const noexcept
    {
        return error_;
    }

    bool NetAddress::operator==(const NetAddress& other) const noexcept
    {
        return (addr_.sin_family == other.addr_.sin_family) &&
               (addr_.sin_addr.s_addr == other.addr_.sin_addr.s_addr) &&
               (addr_.sin_port == other.addr_.sin_port);
    }

    void NetAddress::reset() noexcept
    {
        std::memset(&addr_, 0, sizeof(sockaddr_in));
        addr_.sin_family = AF_INET;
        addr_.sin_addr.s_addr = INADDR_ANY;
        addr_.sin_port = 0;
        error_ = 0;
    }
}
