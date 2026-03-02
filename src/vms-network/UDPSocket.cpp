/*
 * Copyright (C) 2026 Manuel Virgilio <real_virgil@yahoo.it>
 *
 * This file is part of vms-network.
 * Licensed under the GNU Lesser General Public License v2.1.
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#include <vms-network/UDPSocket.h>

#include <cerrno>
#include <sys/socket.h>

namespace vms::network
{
    UdpSocket::UdpSocket() noexcept
        : UdpSocket(kDefaultReceiveBufferSize)
    {
    }

    UdpSocket::UdpSocket(std::size_t receive_buffer_size) noexcept
        : NetSocket(NetSocketType::UDP)
        , receive_buffer_size_(receive_buffer_size)
        , last_error_(0)
    {
    }

    bool UdpSocket::init() noexcept
    {
        const bool ok = NetSocket::init();
        if (!ok) {
            last_error_ = NetSocket::error();
            return false;
        }

        last_error_ = 0;
        return true;
    }

    bool UdpSocket::send(const NetPacket& packet, const NetAddress& to) noexcept
    {
        const ssize_t sent_bytes = ::sendto(
            fd(),
            packet.data(),
            packet.used(),
            0,
            to.sockaddr_ptr(),
            static_cast<socklen_t>(to.size())
        );

        if (sent_bytes == -1) {
            last_error_ = errno;
            return false;
        }

        last_error_ = 0;
        return true;
    }

    bool UdpSocket::receive(NetPacket& packet, NetAddress& from) noexcept
    {
        socklen_t address_size = static_cast<socklen_t>(from.size());
        packet.resize(receive_buffer_size_);

        const ssize_t received_bytes = ::recvfrom(
            fd(),
            packet.data(),
            packet.size(),
            0,
            from.sockaddr_ptr(),
            &address_size
        );

        if (received_bytes == -1) {
            last_error_ = errno;
            return false;
        }

        packet.set_used(static_cast<std::size_t>(received_bytes));
        last_error_ = 0;
        return true;
    }

    int UdpSocket::last_error() const noexcept
    {
        return last_error_;
    }
}
