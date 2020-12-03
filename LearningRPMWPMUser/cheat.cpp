#include "cheat.hpp"

void cheat::run()
{
	std::cout << "[kernel-csgo] Looking for csgo.exe..." << std::endl;
	ULONG process_id = memory::get_process_id("csgo.exe");
	if (process_id) {
		std::cout << "[kernel-csgo] PID found: " << process_id << std::endl;
		std::cout << "[kernel-csgo] Looking for client.dll..." << std::endl;
		ULONG client_base = driver_interface::get_client_base(process_id);
		if (client_base) {
			printf("[kernel-csgo] client.dll found 0x%x\n", client_base);
		}
	}
}
