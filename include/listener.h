/***
* MIT License
*
* Copyright (c) 2025 MouBieCat
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
***/

#pragma once
#ifndef _LISTENER_H_
#define _LISTENER_H_

#include <cstddef>
#include <cstdint>
#include <functional>

namespace cat {
	/*
	 * Represents a simplified network packet received from or sent to a peer.
	 * Encapsulates the peer reference, a pointer to the raw data, and the length of the data.
	 * This struct abstracts away the underlying ENetEvent or ENetPacket details.
	 */
	struct peer_packet {
		void*			peer;	/* Opaque pointer to the remote peer */
		std::uint8_t*	data;	/* Pointer to the raw packet data */
		std::size_t		length;	/* Length of the data in bytes */
	};

	/*
	 * Handler structure for binding callback functions to network events.
	 * Allows client or server code to specify behavior for connect, disconnect, and receive events.
	 */
	struct packet_handler {
		using const_packet = const peer_packet&;

		std::function<void(const_packet)> OnConnect;	/* Called when a peer successfully connects */
		std::function<void(const_packet)> OnDisconnect; /* Called when a peer disconnects */
		std::function<void(const_packet)> OnReceive;	/* Called when a data packet is received from a peer */
	};

	using listen_interface = packet_handler const*;
}

namespace cat::core {

	/*
	 * Installs the global packet handler interface.
	 *
	 * This function registers a user-provided handler used to process
	 * network events (connect, disconnect, receive). The caller must ensure
	 * that the provided pointer remains valid for the duration of use.
	 *
	 * @param _Protocol Pointer to a valid packet_handler instance.
	 *                  Must not be null.
	 */
	void InstallListener(listen_interface _Protocol) noexcept;
}

#endif // ^^^ !_LISTENER_H_