#include "cheat.hpp"


void cheat::run()
{
	std::cout << "[-] Looking for csgo.exe..." << std::endl;
	ULONG process_id = memory::get_process_id("csgo.exe");
	while (process_id == NULL) {
		process_id = memory::get_process_id("csgo.exe");
	}

	if(process_id) {
		std::cout << "[+] PID found: " << process_id << std::endl;
		std::cout << "[-] Looking for client.dll..." << std::endl;
		ULONG client_base = driver_interface::get_client_base(process_id);
		while (client_base == NULL) {
			client_base = driver_interface::get_client_base(process_id);
			Sleep(10);
		}

		if (client_base) {
			printf("[+] client.dll found 0x%x\n", client_base);
			printf("[-] Looking for localplayer entity..\n");
			ULONG localPlayer = driver_interface::read_memory<ULONG>(process_id, client_base + hazedumper::signatures::dwLocalPlayer);
			while (localPlayer == NULL) {
				localPlayer = driver_interface::read_memory<ULONG>(process_id, client_base + hazedumper::signatures::dwLocalPlayer);
				Sleep(10);
			}
			printf("[+] localplayer found at 0x%x\n", localPlayer);
			while (true) {
				localPlayer = driver_interface::read_memory<ULONG>(process_id, client_base + hazedumper::signatures::dwLocalPlayer);
				if (localPlayer) {
					DWORD is_jumping = driver_interface::read_memory<DWORD>(process_id, localPlayer + hazedumper::netvars::m_fFlags);
					if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (is_jumping & (1 << 0))) {
						driver_interface::write_memory<uintptr_t>(process_id, (client_base + hazedumper::signatures::dwForceJump), 6);
					}
				}
				Sleep(10);
			}
		}
	}
}
