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
					glowObject = driver_interface::read_memory<uintptr_t>(process_id, client_base + hazedumper::signatures::dwGlowObjectManager);
					ULONG is_jumping = driver_interface::read_memory<ULONG>(process_id, localPlayer + hazedumper::netvars::m_fFlags);
					ULONG myTeam = driver_interface::read_memory<ULONG>(process_id, localPlayer + hazedumper::netvars::m_iTeamNum);
					//if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (is_jumping & (1 << 0))) {
					//	driver_interface::write_memory<uintptr_t>(process_id, (client_base + hazedumper::signatures::dwForceJump), 6);
					//}

					for (short int i = 0; i < 64; i++) {
						uintptr_t entity = driver_interface::read_memory<uintptr_t>(process_id, client_base + hazedumper::signatures::dwEntityList + i * 0x10);
						//ULONG entity = driver_interface::read_memory<ULONG>(process_id, client_base + hazedumper::signatures::dwEntityList + i * 0x10);
						if (entity != NULL) {
							//std::cout << "ent" << entity << std::endl;
							INT glowIndex = driver_interface::read_memory<int>(process_id, entity + hazedumper::netvars::m_iGlowIndex);
							INT entTeam = driver_interface::read_memory<int>(process_id, entity + hazedumper::netvars::m_iTeamNum);

							if (entTeam == myTeam) {
								//std::cout << "yes" << std::endl;
								driver_interface::write_memory<float>(process_id, glowObject + (glowIndex * (INT)0x38 + (INT)0x4), 0.f);
								driver_interface::write_memory<float>(process_id, glowObject + (glowIndex * (INT)0x38 + (INT)0x8), 0.f);
								driver_interface::write_memory<float>(process_id, glowObject + (glowIndex * (INT)0x38 + (INT)0xC), 0.f);
								driver_interface::write_memory<float>(process_id, glowObject + (glowIndex * (INT)0x38 + (INT)0x10), 1.7f);
							}
							else {
								driver_interface::write_memory<float>(process_id, glowObject + (glowIndex * 0x38) + 0x4, 1);
								driver_interface::write_memory<float>(process_id, glowObject + (glowIndex * 0x38) + 0x8, 0);
								driver_interface::write_memory<float>(process_id, glowObject + (glowIndex * 0x38) + 0xC, 0);
								driver_interface::write_memory<float>(process_id, glowObject + (glowIndex * 0x38) + 0x10, 1.7);
							}
							driver_interface::write_memory<bool>(process_id, glowObject + (glowIndex * 0x38) + 0x24, true);
							driver_interface::write_memory<bool>(process_id, glowObject + (glowIndex * 0x38) + 0x24, false);
						}
						else {
							std::cout << "nope" << std::endl;
							break;
						}
					}
				}
				Sleep(10);
			}
		}
	}
}

