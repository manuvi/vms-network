/*
 * Copyright (C) 2026 Manuel Virgilio <real_virgil@yahoo.it>
 *
 * This file is part of vms-network.
 * Licensed under the GNU Lesser General Public License v2.1.
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#include <vms-network/NetSocket.h>

#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

namespace vms::network
{
    NetSocket::NetSocket(NetSocketType type) noexcept
        : socket_fd_(-1)
        , type_(type)
        , error_(0)
    {
    }

    NetSocket::NetSocket(NetSocketType type, int socket_fd) noexcept
        : socket_fd_(socket_fd)
        , type_(type)
        , error_(0)
    {
    }

    NetSocket::NetSocket(NetSocket&& other) noexcept
        : socket_fd_(other.socket_fd_)
        , type_(other.type_)
        , error_(other.error_)
    {
        other.socket_fd_ = -1;
        other.error_ = 0;
    }

    NetSocket& NetSocket::operator=(NetSocket&& other) noexcept
    {
        if (this != &other) {
            close();
            socket_fd_ = other.socket_fd_;
            type_ = other.type_;
            error_ = other.error_;

            other.socket_fd_ = -1;
            other.error_ = 0;
        }
        return *this;
    }

    NetSocket::~NetSocket()
    {
        close();
    }

    bool NetSocket::init() noexcept
    {
        socket_fd_ = socket(AF_INET, static_cast<int>(type_), 0);
        if (socket_fd_ == -1) {
            error_ = errno;
            return false;
        }
        error_ = 0;
        return true;
    }

    void NetSocket::close() noexcept
    {
        if (socket_fd_ != -1) {
            if (::close(socket_fd_) != 0) {
                error_ = errno;
            } else {
                error_ = 0;
            }
            socket_fd_ = -1;
        }
    }

    bool NetSocket::bind(const NetAddress& address) noexcept
    {
        if (::bind(socket_fd_, address.sockaddr_ptr(), address.size()) != 0) {
            error_ = errno;
            return false;
        }
        error_ = 0;
        return true;
    }

    bool NetSocket::set_non_blocking() noexcept
    {
        const int flags = fcntl(socket_fd_, F_GETFL, 0);
        if (flags < 0) {
            error_ = errno;
            return false;
        }

        if (fcntl(socket_fd_, F_SETFL, flags | O_NONBLOCK) < 0) {
            error_ = errno;
            return false;
        }
        error_ = 0;
        return true;
    }

    int NetSocket::error() const noexcept
    {
        return error_;
    }

    int NetSocket::fd() const noexcept
    {
        return socket_fd_;
    }
}
