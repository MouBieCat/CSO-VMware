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
#ifndef _CORE_H_
#define _CORE_H_

#include <chrono>
#include <cstdint>
#include <string_view>

namespace cat::core {
	/*
	 * Determines whether the currently active ENet host
	 * was initialized as a server instance.
	 * 
	 * @return false - if ENet is uninitialized, has no host, or is a client.
	 */
	[[nodiscard]] bool Core_server() noexcept;

	/*
	 * Initializes the ENet library.
	 * Must be called before creating any ENet hosts or peers.
	 * Throws std::runtime_error if initialization fails.
	 */
	void Core_enet_initialize();

	/*
	 * Shuts down the ENet system and releases all resources.
	 * Should be called only after all ENet hosts and peers have been destroyed.
	 */
	void Core_enet_deinitialize();

	/*
	 * Initializes and creates an ENet server host bound to the specified address and port.
	 * The server will listen for incoming client connections.
	 *
	 * @param _Host    The hostname or IP address to bind the server to.
	 * @param _Port    The port number to listen on.
	 * @param _Channel The number of channels allocated per connected client.
	 * @param _Client  The maximum number of simultaneous client connections.
	 */
	void Core_enet_server_create(const std::string_view _Host, std::uint32_t _Port, std::uint32_t _Channel, std::uint32_t _Client);

	/*
	 * Initializes and creates an ENet client host for connecting to a remote server.
	 *
	 * @param _Channel The number of channels allocated for communication with the server.
	 */
	void Core_enet_client_create(std::uint32_t _Channel);

	/*
	 * Connects the client host to a remote ENet server.
	 *
	 * Establishes a peer connection to the specified server address and port,
	 * requesting the given number of channels for communication. The actual
	 * number of usable channels must not exceed the host's maximum channels.
	 *
	 * @param _Server  The remote server address (IP or hostname) to connect to.
	 * @param _Port    The port number of the remote ENet server.
	 * @param _Channel The number of channels to use for this connection.
	 *                 Should be <= the client host's max channel count.
	 */
	void Core_enet_client_connect(const std::string_view _Server, std::uint32_t _Port, std::uint32_t _Channel);

	/**
	 * Sends a data packet from the server to a specific connected client.
	 *
	 * @param _Peer     Pointer to the ENetPeer representing the target client.
	 * @param _Data     Pointer to the raw data buffer to be sent.
	 * @param _Size     Size of the data buffer in bytes.
	 * @param _Channel  Channel ID used for sending the packet.
	 * @param _Flags    ENet packet flags.
	 */
	void Core_enet_server_send(void* _Peer, const void* _Data, std::size_t _Size, std::uint32_t _Channel, std::uint32_t _Flags);

	/**
	 * Sends a data packet from the client to the connected ENet server.
	 *
	 * @param _Data     Pointer to the raw data buffer to be sent.
	 * @param _Size     Size of the data buffer in bytes.
	 * @param _Channel  Channel ID used for sending the packet.
	 * @param _Flags    ENet packet flags.
	 */
	void Core_enet_client_send(const void* _Data, std::size_t _Size, std::uint32_t _Channel, std::uint32_t _Flags);

	/**
	 * Polls the ENet host for network events.
	 *
	 * @param _Timeout Duration to wait for an event. Use std::chrono types like
	 *                 std::chrono::milliseconds, std::chrono::seconds, etc.
	 */
	void Core_enet_pollevents(std::chrono::milliseconds _Timeout);
}

#endif // ^^^ !_CORE_H_