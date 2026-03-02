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

namespace vms::network
{
    /**
     * @brief UDP socket wrapper built on top of `NetSocket`.
     *
     * Provides datagram send/receive with `NetPacket` payloads.
     */
    class UdpSocket : public NetSocket
    {
    public:
        static constexpr std::size_t kDefaultReceiveBufferSize = 1500;

        UdpSocket() noexcept;
        explicit UdpSocket(std::size_t receive_buffer_size) noexcept;
        ~UdpSocket() override = default;

        /// Creates underlying UDP descriptor.
        bool init() noexcept override;

        /// Sends current packet payload to destination endpoint.
        bool send(const NetPacket& packet, const NetAddress& to) noexcept;
        /// Receives one datagram into packet and fills sender endpoint.
        bool receive(NetPacket& packet, NetAddress& from) noexcept;

        /// Last UDP operation error code.
        int last_error() const noexcept;

    private:
        std::size_t receive_buffer_size_;
        int last_error_;
    };
}
