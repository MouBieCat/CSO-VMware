#include <cassert>
#include <stdexcept>
#include <enet/enet.h>
#include "core.h"
#include "listener.h"

/*
 * Directly instructs the linker to include required libraries.
 * Useful for Windows builds where certain system or third-party libraries
 * are needed for networking or timing functions.
 */
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")

namespace cat::core {
	/*
		Global flag indicating whether the current ENet host
		was initialized in server mode.
	 */
	static bool server = false;

	/*
		Indicates whether the ENet library has been successfully initialized.
		Prevents redundant initialization and ensures a proper shutdown sequence.
	 */
	static bool initialized = false;

	/*
		Global pointer to the active ENet host instance.
		Used by both server and client configurations to manage network operations.
		Set to nullptr when no active host exists.
	 */
	static ENetHost* host = nullptr;

	/*
		Global pointer representing the active peer connection.
		Used on the client side to maintain the connection to a remote ENet server.
	 */
	static ENetPeer* conn = nullptr;


	/*
		Determines whether the currently active ENet host
		was initialized as a server instance.
		
		@return false - if ENet is uninitialized, has no host, or is a client.
	 */
	bool
	Core_server() noexcept {
		return server;
	}

	/*
		Initializes the ENet library.
		Must be called before creating any ENet hosts or peers.
		Throws std::runtime_error if initialization fails.
	 */
	void
	Core_enet_initialize() {
		int res = enet_initialize();
		if (res != 0) {
			throw std::runtime_error("An error occurred while initializing ENet.");
		}

		initialized = true;
	}

	/*
		Initializes and creates an ENet server host bound to the specified address and port.
		The server will listen for incoming client connections.
		
		@param _Host    The hostname or IP address to bind the server to.
		@param _Port    The port number to listen on.
		@param _Channel The number of channels allocated per connected client.
		@param _Client  The maximum number of simultaneous client connections.
	 */
	void
	Core_enet_server_create(const std::string_view _Host, std::uint32_t _Port, std::uint32_t _Channel, std::uint32_t _Client) {
		if (!initialized) {
			throw std::runtime_error("ENet must be initialized before creating a server.");
		}

		if (host != nullptr) {
			throw std::runtime_error("ENet host already created.");
		}

		ENetAddress addr;
		enet_address_set_host(&addr, _Host.data());
		addr.port = _Port;

		host = enet_host_create(&addr, _Client, _Channel, NULL, NULL);
		if (host == nullptr) {
			throw std::runtime_error("An error occurred while trying to create an ENet server host.");
		}

		server = true;
	}

	/*
		Initializes and creates an ENet client host for connecting to a remote server.
		
		@param _Channel The number of channels allocated for communication with the server.
	 */
	void
	Core_enet_client_create(std::uint32_t _Channel) {
		if (!initialized) {
			throw std::runtime_error("ENet must be initialized before creating a client.");
		}

		if (host != nullptr) {
			throw std::runtime_error("ENet host already created.");
		}

		host = enet_host_create(NULL, 1, _Channel, NULL, NULL);
		if (host == nullptr) {
			throw std::runtime_error("An error occurred while trying to create an ENet client host.");
		}

		server = false;
	}

	/*
		Establishes a connection from the client host to a remote ENet server.
		
		This function must be called only after a client host has been created
		using `_Core_enet_client_create()`. The connection attempt is asynchronous;
		you must handle ENET_EVENT_TYPE_CONNECT in your event loop to confirm success.
		
		@param _Server  The target server's hostname or IP address.
		@param _Port    The target server's port number.
		@param _Channel The number of channels (must match the one used when creating the host).
	 */
	void
	Core_enet_client_connect(const std::string_view _Server, std::uint32_t _Port, std::uint32_t _Channel) {
		if (!initialized) {
			throw std::runtime_error("ENet must be initialized before creating a client.");
		}

		if (host == nullptr) {
			throw std::runtime_error("ENet client host has not been created.");
		}

		ENetAddress addr;
		enet_address_set_host(&addr, _Server.data());
		addr.port = _Port;

		conn = enet_host_connect(host, &addr, _Channel, NULL);
		if (conn == nullptr) {
			throw std::runtime_error("Failed to initiate connection to remote server.");
		}
	}

	/**
		Sends a data packet from the server to a specific connected client.
		
		@param _Peer     Pointer to the ENetPeer representing the target client.
		@param _Data     Pointer to the raw data buffer to be sent.
		@param _Size     Size of the data buffer in bytes.
		@param _Channel  Channel ID used for sending the packet.
		@param _Flags    ENet packet flags.
	 */
	void
	Core_enet_server_send(void* _Peer, const void* _Data, std::size_t _Size, std::uint32_t _Channel, std::uint32_t _Flags) {
		assert(host && "ENet host must be created before sending packets.");
		assert(server && "This function is only valid for server instances.");
		assert(initialized && "ENet must be initialized before sending packets.");

		ENetPeer* peer = static_cast<ENetPeer*>(_Peer);
		ENetPacket* pkt = enet_packet_create(_Data, _Size, _Flags);
		enet_peer_send(peer, _Channel, pkt);
		enet_host_flush(host);
	}

	/**
		Sends a data packet from the client to the connected ENet server.
		
		@param _Data     Pointer to the raw data buffer to be sent.
		@param _Size     Size of the data buffer in bytes.
		@param _Channel  Channel ID used for sending the packet.
		@param _Flags    ENet packet flags.
	 */
	void
	Core_enet_client_send(const void* _Data, std::size_t _Size, std::uint32_t _Channel, std::uint32_t _Flags) {
		assert(host && "ENet host must be created before sending packets.");
		assert(conn && "ENet client must be connected before sending packets.");
		assert(initialized && "ENet must be initialized before sending packets.");

		ENetPacket* pkt = enet_packet_create(_Data, _Size, _Flags);
		enet_peer_send(conn, _Channel, pkt);
		enet_host_flush(host);
	}
	
	/**
		Polls the ENet host for network events.
		
		@param _Timeout Duration to wait for an event. Use std::chrono types like
		                std::chrono::milliseconds, std::chrono::seconds, etc.
	 */
	void
	Core_enet_pollevents(std::chrono::milliseconds _Timeout) {
		const enet_uint32 timeout = static_cast<enet_uint32>(_Timeout.count());

		ENetEvent event;
		while (enet_host_service(host, &event, timeout) > 0) {
			enet_packet_destroy(event.packet);
			event.packet = nullptr;
		}
	}

	/*
		Shuts down the ENet system and releases all resources.
		Should be called only after all ENet hosts and peers have been destroyed.
	 */
	void
	Core_enet_deinitialize() {
		if (host) {
			enet_host_destroy(host);
			host = nullptr;
		}

		if (initialized) {
			enet_deinitialize();
			initialized = false;
		}

		conn = nullptr;
		server = false;
	}
}