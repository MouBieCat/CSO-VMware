#include <iostream>
#include <Windows.h>
#include "net.h"
#include "client.h"
#include "server.h"
#include "listener.h"

#include "core.h"

#define CLIENT 1

std::vector<void*> peers;

void OnConnect(cat::peer_data _Data) {
#if CLIENT
#else
	peers.push_back(_Data.peer);
#endif // CLIENT
	std::cout << "> OnConnect" << std::endl;
}

void OnDisconnect(cat::peer_data _Data) {
	std::cout << "> OnDisconnect" << std::endl;
#if CLIENT
#else
	peers.erase(std::remove(peers.begin(), peers.end(), _Data.peer), peers.end());
#endif // CLIENT
}

void OnReceive(cat::peer_data _Data) {
	std::cout << "> OnReceive < " << _Data.data << " >" << std::endl;
#if CLIENT
#else
#endif // CLIENT
}

int main(int _Argc, char** _Argv) {
#if CLIENT
	cat::client host("localhost", 2330, "moubiecat");
#else
	cat::server host("localhost", 2330);
#endif // CLIENT
	host.connect();

	while (cat::is_connecting()) {
		host.poll();

#if CLIENT
		constexpr const char* msg = "client: hello, world\0";
		cat::core::Core_enet_client_send(msg, std::strlen(msg) + 1, 0, NULL);

		if (GetAsyncKeyState(VK_F1)) {
			host.connect();
		}
		if (GetAsyncKeyState(VK_F2)) {
			host.disconnect();
		}
#else
		if (GetAsyncKeyState(0x45)) {
			for (void* peer : peers) {
				constexpr const char* msg = "server: hello, world\0";
				cat::core::Core_enet_server_send(peer, msg, std::strlen(msg) + 1, 0, NULL);
			}
		}
#endif // CLIENT
	}

	host.shutdown();
	return 0;
}