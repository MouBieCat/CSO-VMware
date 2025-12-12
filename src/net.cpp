#include <chrono>
#include "core.h"
#include "client.h"
#include "server.h"
#include "stream.h"

namespace cat {
	/*
		Static flag indicating whether the system or host is currently active.
		Can be used to control the main loop or guard against operations
		when the system is inactive.
	 */
	static bool flag = false;


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
		flag = true;
	}

	/*
		Disconnects the client from the currently connected ENet server.
		providing a convenient method to terminate the connection from the client class.
	 */
	void
	client::disconnect() const {
		core::Core_enet_client_disconnect(false);
	}
	
	/*
		Polls the ENet server for incoming events such as connections,
		disconnections, and data packets. This function blocks for up to
		600 ms while waiting for events, then returns control to the caller.
	 */
	void
	client::poll() const noexcept {
		using namespace std::chrono;
		core::Core_enet_pollevents(600ms);
	}

	/*
		Sends a serialized packet over the network.
		
		@param _Packet Reference to a packet object to be serialized and sent.
	 */
	void
	client::send(const packet& _Packet) const noexcept {
		cat::ostream os;
		_Packet.serialize(os);
		core::Core_enet_client_send(os.buffer().data(), os.size(), 0, NULL);
	}

	/*
		Safely shuts down the client or server instance.

		This function ensures that all active connections are properly disconnected
		and that any associated ENet resources are released. It is safe to call
		multiple times; subsequent calls will have no effect.
	 */
	void
	client::shutdown() const noexcept {
		flag = false;
		core::Core_enet_deinitialize();
	}

	/*
		Checks whether the main event loop of the host is currently active.

		@return true if the loop is running and the host is processing events;
				false if the loop has been stopped or the host is shutting down.
	 */
	bool
	client::loop_active() noexcept {
		return flag;
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
		flag = true;
	}

	/*
		Polls the ENet server for incoming events such as connections,
		disconnections, and data packets. This function blocks for up to
		600 ms while waiting for events, then returns control to the caller.
	 */
	void
	server::poll() const noexcept {
		using namespace std::chrono;
		core::Core_enet_pollevents(600ms);
	}

	/*
		Sends a serialized packet to a specific peer.
		
		@param _Peer   Pointer to the target peer connection.
		@param _Packet Reference to the packet object to serialize and send.
	 */
	void
	server::send(void* _Peer, const packet& _Packet) const noexcept {
		cat::ostream os;
		_Packet.serialize(os);
		core::Core_enet_server_send(_Peer, os.buffer().data(), os.size(), 0, NULL);
	}

	/*
		Safely shuts down the client or server instance.
		
		This function ensures that all active connections are properly disconnected
		and that any associated ENet resources are released. It is safe to call
		multiple times; subsequent calls will have no effect.
	 */
	void
	server::shutdown() const noexcept {
		flag = false;
		core::Core_enet_deinitialize();
	}

	/*
		Checks whether the main event loop of the host is currently active.
		
		@return true if the loop is running and the host is processing events;
		        false if the loop has been stopped or the host is shutting down.
	 */
	bool
	server::loop_active() noexcept {
		return flag;
	}
}