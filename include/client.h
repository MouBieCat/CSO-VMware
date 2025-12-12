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
		 */
		constexpr client(const std::string_view _Server, std::uint32_t _Port) noexcept
			: server(_Server), port(_Port) {}

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
		 * Performs the necessary initialization and setup for the client or server.
		 * This may include creating network hosts, initializing internal state, and
		 * preparing the object for sending/receiving data.
		 *
		 * @return true  If the setup was successful
		 * @return false If any step failed
		 */
		[[nodiscard]] bool install();

		/*
		 * Establishes a connection from the client to the specified server.
		 * This function should be called after `install()` has successfully completed.
		 * It initiates an asynchronous connection; success/failure should be
		 * confirmed through event handling or a connected status check.
		 *
		 * @return true  If the connection request was successfully initiated
		 * @return false If the connection could not be started (e.g., host not created)
		 */
		[[nodiscard]] bool connect();
	private:
		// Host name or IP address of the remote server
		const std::string_view server;

		// Port number on the remote server that the client should connect to
		const std::uint32_t port;
	};
}

#endif // ^^^ !_CLIENT_H_