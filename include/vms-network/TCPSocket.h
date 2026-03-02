/*
 * Copyright (C) 2026 Manuel Virgilio <real_virgil@yahoo.it>
 *
 * This file is part of vms-network.
 * Licensed under the GNU Lesser General Public License v2.1.
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#pragma once

#include <vms-network/NetAddress.h>
#include <vms-network/NetPacket.h>
#include <vms-network/NetSocket.h>

#include <cstddef>
#include <vector>

namespace vms::network
{
    /**
     * @brief TCP client-side socket wrapper built on `NetSocket`.
     *
     * Uses an internal receive buffer (MTU-sized) and packet-based APIs
     * for send/receive integration.
     */
    class TcpSocket : public NetSocket
    {
    public:
        static constexpr std::size_t kDefaultMtu = 1500;

        TcpSocket() noexcept;
        explicit TcpSocket(int socket_fd) noexcept;
        TcpSocket(const TcpSocket&) = delete;
        TcpSocket& operator=(const TcpSocket&) = delete;
        TcpSocket(TcpSocket&& other) noexcept = default;
        TcpSocket& operator=(TcpSocket&& other) noexcept = default;
        ~TcpSocket() override = default;

        /// Sets internal receive buffer size used by `receive`.
        void set_mtu(std::size_t mtu) noexcept;
        /// Connects socket to remote endpoint.
        bool connect(const NetAddress& endpoint) noexcept;

        /// Last TCP operation error code.
        int last_error() const noexcept;

        /**
         * @brief Sends packet payload.
         * @return false on send error or partial send.
         */
        bool send(const NetPacket& packet) noexcept;
        /// Receives bytes into packet starting at offset 0.
        bool receive(NetPacket& packet) noexcept;

    private:
        std::vector<uint8_t> receive_buffer_;
        int last_error_;
        std::size_t mtu_;
    };
}
