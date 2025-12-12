#include <chrono>
#include "core.h"
#include "server.h"
#include "client.h"

namespace cat {
	/*
		Performs the necessary initialization and setup for the client or server.
		This may include creating network hosts, initializing internal state, and
		preparing the object for sending/receiving data.
	 */
	void
	client::connect() const {
		core::Core_enet_initialize();
		core::Core_enet_client_create(1);
		core::Core_enet_client_connect(server, port, 0);
	}
	
	/*
		Polls the ENet server for incoming events such as connections,
		disconnections, and data packets. This function blocks for up to
		600 ms while waiting for events, then returns control to the caller.
	 */
	void
	client::poll() const {
		using namespace std::chrono;
		core::Core_enet_pollevents(600ms);
	}

	/*
		Safely shuts down the client or server instance.

		This function ensures that all active connections are properly disconnected
		and that any associated ENet resources are released. It is safe to call
		multiple times; subsequent calls will have no effect.
	 */
	void
	client::shutdown() const noexcept {
		core::Core_enet_deinitialize();
	}

	/*
		Performs the necessary initialization and setup for the client or server.
		This may include creating network hosts, initializing internal state, and
		preparing the object for sending/receiving data.
	 */
	void
	server::connect() const {
		core::Core_enet_initialize();
		core::Core_enet_server_create(host, port, 1, 32);
	}

	/*
		Polls the ENet server for incoming events such as connections,
		disconnections, and data packets. This function blocks for up to
		600 ms while waiting for events, then returns control to the caller.
	 */
	void
	server::poll() const {
		using namespace std::chrono;
		core::Core_enet_pollevents(600ms);
	}

	/*
		Safely shuts down the client or server instance.
		
		This function ensures that all active connections are properly disconnected
		and that any associated ENet resources are released. It is safe to call
		multiple times; subsequent calls will have no effect.
	 */
	void
	server::shutdown() const noexcept {
		core::Core_enet_deinitialize();
	}
}