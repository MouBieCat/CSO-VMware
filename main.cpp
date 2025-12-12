#include <server.h>

int main(int _Argc, char** _Argv) {
	cat::server server("localhost", 2330);
	server.install();

	return 0;
}