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
#ifndef _SERVER_H_
#define _SERVER_H_

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

namespace cat {
	/*
	 * The server class represents a simple immutable descriptor for a network
	 * listening endpoint. It stores the host address and port number but does not
	 * perform any socket or ENet initialization. The caller is responsible for
	 * using these parameters when creating the underlying network host.
	 */
	class server {
	public:
		/*
		 * Constructs a server endpoint descriptor with the specified host and port.
		 *
		 * The constructor performs no validation and does not establish any
		 * networking resources. It simply stores the provided parameters.
		 *
		 * @param _Host  A string view representing the host/IP on which the server
		 *               should listen. The referenced string must remain valid for
		 *               the lifetime of this object.
		 *
		 * @param _Port  The port number on which the server is expected to bind.
		 */
		constexpr server(const std::string_view _Host, std::uint32_t _Port) noexcept
			: host(_Host), port(_Port) {}

		/*
		 * Returns the client/server address in "host:port" format.
		 * Each call generates a new std::string using std::format.
		 *
		 * @return A std::string representing the formatted address.
		 */
		[[nodiscard]] std::string ipaddress() const noexcept {
			return std::format("{}:{}", host, port);
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
	private:
		// Host name or IP address for the listening endpoint
		const std::string_view host;

		// Port number for accepting incoming connections
		const std::uint32_t port;
	};
}

#endif // ^^^ !_SERVER_H_