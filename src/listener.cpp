#include <cassert>
#include "listener.h"

namespace cat {
	/*
		Global pointer to the currently installed listener interface.
		
		This pointer references the handler structure that contains user-defined
		callback functions for connection, disconnection, and packet-receive events.
	 */
	static listen_interface listener;


	/*
		Installs the global packet handler interface.
		
		This function registers a user-provided handler used to process
		network events (connect, disconnect, receive). The caller must ensure
		that the provided pointer remains valid for the duration of use.
		
		@param _Protocol Pointer to a valid packet_handler instance.
		                 Must not be null.
	 */
	void InstallListener(listen_interface _Protocol) noexcept {
		assert(_Protocol && "Listener protocol must not be null.");
		listener = _Protocol;
	}

	/*
		Retrieves the currently installed packet handler.
		
		If no handler has been installed, this function returns nullptr.
		The caller should check the returned value before use.
		
		@return The installed packet_handler interface, or nullptr if
		        no handler has been registered.
	 */
	[[nodiscard]] listen_interface LocateListener() noexcept {
		assert(listener && "Listener must be installed before locating.");
		return listener;
	}
}