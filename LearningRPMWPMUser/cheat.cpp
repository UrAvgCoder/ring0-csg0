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
			uintptr_t glowObject = driver_interface::read_memory<uintptr_t>(process_id, client_base + hazedumper::signatures::dwGlowObjectManager);
			while (localPlayer == NULL) {
				localPlayer = driver_interface::read_memory<ULONG>(process_id, client_base + hazedumper::signatures::dwLocalPlayer);
				Sleep(10);
			}
			printf("[+] localplayer found at 0x%x\n", localPlayer);
			while (true) {
				localPlayer = driver_interface::read_memory<ULONG>(process_id, client_base + hazedumper::signatures::dwLocalPlayer);
				if (localPlayer) {
					ULONG is_jumping = driver_interface::read_memory<ULONG>(process_id, localPlayer + hazedumper::netvars::m_fFlags);
					ULONG myTeam = driver_interface::read_memory<ULONG>(process_id, localPlayer + hazedumper::netvars::m_iTeamNum);
					if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (is_jumping & (1 << 0))) {
						driver_interface::write_memory<uintptr_t>(process_id, (client_base + hazedumper::signatures::dwForceJump), 6);
					}

					if (!GetAsyncKeyState(VK_INSERT)) {
						for (short int i = 0; i < 64; i++) {
							ULONG entity = driver_interface::read_memory<ULONG>(process_id, client_base + hazedumper::signatures::dwEntityList + i * 0x10);
							if (entity != NULL) {
								int glowIndex = driver_interface::read_memory<int>(process_id, entity + hazedumper::netvars::m_iGlowIndex);
								int entTeam = driver_interface::read_memory<int>(process_id, entity + hazedumper::netvars::m_iTeamNum);

								if (entTeam == myTeam) {
									SGlowStruct teamGlow = driver_interface::read_memory<SGlowStruct>(process_id, glowIndex * 0x38);
									teamGlow.blue = 1.0f;
									teamGlow.alpha = 1.0f;
									teamGlow.renderwhenocculed = true;
									teamGlow.renderwhenunocculed = false;
									driver_interface::write_memory_struct(process_id, (glowIndex * 0x38), teamGlow);
								}
								else {

								}
							}
						}
					}
				}
				Sleep(10);
			}
		}
	}
}
