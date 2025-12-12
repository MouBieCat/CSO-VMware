#include <chrono>
#include "core.h"
#include "client.h"
#include "server.h"
#include "stream.h"

namespace cat {
	/*
		Global flag indicating whether the ENet system has been initialized
		and the host (server or client) has been created. This is shared
		between server and client code to guard network operations.
	 */
	static bool setup = false;

	/*
		Global flag indicating whether a client connection to a remote server
		is currently active or in progress. Shared between client/server code
		to track connection state.
	 */
	static bool connecting = false;


	/*
		Reports whether the ENet subsystem has been successfully initialized.
		
		@return true  If the ENet system is initialized and ready.
		@return false If ENet has not been set up or has already been shut down.
	 */
	bool
	is_setup() noexcept {
		return setup;
	}

	/*
		Returns whether a client connection attempt is currently active.
		
		@return true  If a connection is active or being established.
		@return false If no client connection exists.
	 */
	bool
	is_connecting() noexcept {
		return connecting;
	}

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
		setup = true;
		connecting = true;
	}

	/*
		Disconnects the client from the currently connected ENet server.
		providing a convenient method to terminate the connection from the client class.
	 */
	void
	client::disconnect() const {
		core::Core_enet_client_disconnect();
		connecting = false;
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
		core::Core_enet_deinitialize();
		setup = false;
		connecting = false;
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
		setup = true;
		connecting = true;
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
		core::Core_enet_deinitialize();
		setup = false;
		connecting = false;
	}
}