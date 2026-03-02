/*
 * Copyright (C) 2026 Manuel Virgilio <real_virgil@yahoo.it>
 *
 * This file is part of vms-network.
 * Licensed under the GNU Lesser General Public License v2.1.
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#include <vms-network/TCPSocket.h>

#include <cerrno>
#include <sys/socket.h>

namespace vms::network
{
    TcpSocket::TcpSocket() noexcept
        : TcpSocket(-1)
    {
    }

    TcpSocket::TcpSocket(int socket_fd) noexcept
        : NetSocket(NetSocketType::TCP, socket_fd)
        , last_error_(0)
        , mtu_(kDefaultMtu)
    {
        receive_buffer_.resize(mtu_);
    }

    void TcpSocket::set_mtu(std::size_t mtu) noexcept
    {
        mtu_ = mtu;
        receive_buffer_.resize(mtu_);
    }

    bool TcpSocket::connect(const NetAddress& endpoint) noexcept
    {
        const int status = ::connect(
            fd(),
            endpoint.sockaddr_ptr(),
            static_cast<socklen_t>(endpoint.size())
        );

        if (status < 0) {
            last_error_ = errno;
            return false;
        }

        last_error_ = 0;
        return true;
    }

    int TcpSocket::last_error() const noexcept
    {
        return last_error_;
    }

    bool TcpSocket::send(const NetPacket& packet) noexcept
    {
        const ssize_t sent_bytes = ::send(fd(), packet.data(), packet.used(), 0);

        if (sent_bytes == -1) {
            last_error_ = errno;
            return false;
        }

        if (static_cast<std::size_t>(sent_bytes) != packet.used()) {
            last_error_ = EAGAIN;
            return false;
        }

        last_error_ = 0;
        return true;
    }

    bool TcpSocket::receive(NetPacket& packet) noexcept
    {
        const ssize_t received_bytes = ::recv(
            fd(),
            receive_buffer_.data(),
            receive_buffer_.size(),
            0
        );

        if (received_bytes < 0) {
            last_error_ = errno;
            return false;
        }

        packet.write(
            0,
            receive_buffer_,
            static_cast<std::size_t>(received_bytes)
        );
        last_error_ = 0;
        return true;
    }
}
