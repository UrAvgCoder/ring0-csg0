#include "cheat.hpp"


void cheat::run()
{
	std::cout << "[-] Looking for csgo.exe..." << std::endl;
	ULONG process_id = memory::get_process_id("csgo.exe");
	
	if (process_id) {
		std::cout << "[+] PID found: " << process_id << std::endl;
		std::cout << "[-] Looking for client.dll..." << std::endl;
		ULONG client_base = driver_interface::get_client_base(process_id);

		if (client_base) {
			printf("[+] client.dll found 0x%x\n", client_base);
			//printf("[kernel-csgo] finding localplayer address..\n");
			ULONG localPlayer = driver_interface::read_memory<ULONG>(process_id, client_base + hazedumper::signatures::dwLocalPlayer);
			printf("[+] localplayer found at 0x%x\n", localPlayer);
			DWORD is_jumping = driver_interface::read_memory<DWORD>(process_id, localPlayer + hazedumper::netvars::m_fFlags);

			// TODO: write-to-memory;

			/*for(;;) {
				DWORD is_jumping = driver_interface::read_memory<DWORD>(process_id, localPlayer + hazedumper::netvars::m_fFlags);
				printf("is_jumping value = %d\n", (is_jumping));
				Sleep(100);
			}*/
		}
	}
}
