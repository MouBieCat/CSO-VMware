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

namespace cat {
	/*
	 * Represents a simplified network packet received from or sent to a peer.
	 * Encapsulates the peer reference, a pointer to the raw data, and the length of the data.
	 * This struct abstracts away the underlying ENetEvent or ENetPacket details,
	 * so upper layers don't need to depend on ENet types.
	 */
	const struct peer_data {
		void*			peer;	/* Opaque pointer to the remote peer */
		std::uint8_t*	data;	/* Pointer to the raw packet data */
		std::size_t		length;	/* Length of the data in bytes */
	};
}

/*
 * Default event callbacks for the network layer.
 * These functions are declared here but must be implemented in client.cpp or server.cpp.
 * They define how the application responds to connection, disconnection, and packet reception events.
 */
extern void OnConnect(cat::peer_data _Data);

/*
 * Default event callbacks for the network layer.
 * These functions are declared here but must be implemented in client.cpp or server.cpp.
 * They define how the application responds to connection, disconnection, and packet reception events.
 */
extern void OnDisconnect(cat::peer_data _Data);

/*
 * Default event callbacks for the network layer.
 * These functions are declared here but must be implemented in client.cpp or server.cpp.
 * They define how the application responds to connection, disconnection, and packet reception events.
 */
extern void OnReceive(cat::peer_data _Data);

#endif // ^^^ !_LISTENER_H_