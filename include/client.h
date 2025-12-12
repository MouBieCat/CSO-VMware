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
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include "packet.h"

namespace cat {
	/*
	 * The client class represents a connection descriptor for reaching a remote
	 * server endpoint. It holds the target server address and port but does not
	 * perform any connection logic by itself. Higher-level networking components
	 * are expected to consume these parameters to initiate an outbound connection.
	 */
	class client {
	public:
		/*
		 * Constructs a client endpoint descriptor pointing to a specific remote server.
		 *
		 * The constructor stores only the server address and port. It does not open
		 * any connection or allocate network resources.
		 *
		 * @param _Server A string view referencing the server host/IP. The referenced
		 *                string must remain valid for the lifetime of this object.
		 *
		 * @param _Port   Destination port number used during the connection attempt.
		 * @param _Name   A user-defined identifier for this client instance
		 */
		constexpr client(const std::string_view _Server, std::uint32_t _Port, std::string_view _Name) noexcept
			: server(_Server), port(_Port), name(_Name) {}

		/*
		 * Returns the client/server address in "host:port" format.
		 * Each call generates a new std::string using std::format.
		 *
		 * @return A std::string representing the formatted address.
		 */
		[[nodiscard]] std::string ipaddress() const noexcept {
			return std::format("{}:{}", server, port);
		}

		/*
		 * Returns the user-defined identity string associated with this client.
		 * 
		 * @return A non-owning string_view representing the client's username.
		 */
		[[nodiscard]] constexpr std::string_view username() const noexcept {
			return name;
		}

		/*
		 * Performs the necessary initialization and setup for the client or server.
		 * This may include creating network hosts, initializing internal state, and
		 * preparing the object for sending/receiving data.
		 */
		void connect() const;

		/*
		 * Polls the ENet server for incoming events such as connections,
		 * disconnections, and data packets. This function blocks for up to
		 * 600 ms while waiting for events, then returns control to the caller.
		 */
		void poll() const noexcept;

		/*
		 * Sends a serialized packet over the network.
		 * 
		 * @param _Packet Reference to a packet object to be serialized and sent.
		 */
		void send(const packet& _Packet) const noexcept;

		/*
		 * Safely shuts down the client or server instance.
		 *
		 * This function ensures that all active connections are properly disconnected
		 * and that any associated ENet resources are released. It is safe to call
		 * multiple times; subsequent calls will have no effect.
		 */
		void shutdown() const noexcept;

		/*
		 * Checks whether the main event loop of the host is currently active.
		 *
		 * @return true if the loop is running and the host is processing events;
		 *         false if the loop has been stopped or the host is shutting down.
		 */
		[[nodiscard]] static bool loop_active() noexcept;
	private:
		// Host name or IP address of the remote server
		const std::string_view server;

		// Port number on the remote server that the client should connect to
		const std::uint32_t port;

		// Local identity tag for this client instance.
		const std::string_view name;
	};
}

#endif // ^^^ !_CLIENT_H_