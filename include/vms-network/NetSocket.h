/*
 * Copyright (C) 2026 Manuel Virgilio <real_virgil@yahoo.it>
 *
 * This file is part of vms-network.
 * Licensed under the GNU Lesser General Public License v2.1.
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#pragma once

#include <sys/socket.h>

#include "NetAddress.h"

namespace vms::network
{
    /// Underlying POSIX socket kind.
    enum class NetSocketType
    {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };

    /**
     * @brief Base RAII wrapper for POSIX socket descriptors.
     *
     * Owns a descriptor, closes it on destruction, and provides shared
     * operations used by concrete TCP/UDP wrappers.
     */
    class NetSocket
    {
    public:
        explicit NetSocket(NetSocketType type) noexcept;
        NetSocket(NetSocketType type, int socket_fd) noexcept;
        NetSocket(const NetSocket&) = delete;
        NetSocket& operator=(const NetSocket&) = delete;
        NetSocket(NetSocket&& other) noexcept;
        NetSocket& operator=(NetSocket&& other) noexcept;

        virtual ~NetSocket();

        /// Creates a new socket descriptor according to `NetSocketType`.
        virtual bool init() noexcept;
        /// Closes owned descriptor if valid.
        void close() noexcept;

        /// Binds descriptor to local endpoint.
        bool bind(const NetAddress& address) noexcept;
        /// Enables non-blocking mode via `fcntl`.
        bool set_non_blocking() noexcept;

        /// Last socket operation error code.
        int error() const noexcept;
        /// Raw file descriptor (`-1` when not initialized/closed).
        int fd() const noexcept;

    protected:
        int socket_fd_;

    private:
        NetSocketType type_;
        int error_;
    };
}
