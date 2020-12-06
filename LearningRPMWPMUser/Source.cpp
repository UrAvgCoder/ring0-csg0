#include "Source.h"

bool isGlow = true;
bool glowTeamCheck = false;

uintptr_t process_id, client_base;

int main() {	
	process_id = memory::get_process_id("csgo.exe");
	std::cout << "csgo pid found: " << process_id << std::endl;
	Sleep(200);
	client_base = driver_interface::get_client_base(process_id);
	while (true) {
		if (isGlow) {
			glowESP();
			bunnyHOP();
		}
	}
}

void glowESP() {
	uintptr_t glowObject = driver_interface::read_memory< uintptr_t>(process_id, client_base + hazedumper::signatures::dwGlowObjectManager);
	int glowObjectCount = driver_interface::read_memory<int>(process_id, client_base + hazedumper::signatures::dwGlowObjectManager + 0x4);

	for (int i = 0; i < glowObjectCount; i++) {
		uintptr_t localPlayer = driver_interface::read_memory<uintptr_t>(process_id, client_base + hazedumper::signatures::dwLocalPlayer);
		int myTeam = driver_interface::read_memory<int>(process_id, localPlayer + hazedumper::netvars::m_iTeamNum);
		int glow_current_ent = driver_interface::read_memory<int>(process_id, client_base + hazedumper::signatures::dwEntityList + (i * 0x10));
		int glow_current_dormant = driver_interface::read_memory<int>(process_id, glow_current_ent + hazedumper::signatures::m_bDormant);
		int glow_current_glow_index = driver_interface::read_memory<int>(process_id, glow_current_ent + hazedumper::netvars::m_iGlowIndex);
		int entTeam = driver_interface::read_memory<int>(process_id, glow_current_ent + hazedumper::netvars::m_iTeamNum);
		float coloronf = 1.0f;
		ULONG coloron = *((unsigned long*)&coloronf);
		float colorofff = 0.0f;
		ULONG coloroff = *((unsigned long*)&colorofff);


		if (glow_current_dormant == 1 || entTeam == 0) continue;
		else {
			if (myTeam == entTeam) {
				driver_interface::write_memory<float>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x4, coloroff);
				driver_interface::write_memory<float>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x8, coloron);
				driver_interface::write_memory<float>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0xC, coloroff);
				driver_interface::write_memory<float>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x10, coloron);
				driver_interface::write_memory<bool>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x24, false);
				driver_interface::write_memory<bool>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x25, true);
			}
			else {
				driver_interface::write_memory<float>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x4, coloron);
				driver_interface::write_memory<float>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x8, coloroff);
				driver_interface::write_memory<float>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0xC, coloroff);
				driver_interface::write_memory<float>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x10, coloron);
				driver_interface::write_memory<bool>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x24, true);
				driver_interface::write_memory<bool>(process_id, glowObject + (glow_current_glow_index * 0x38) + 0x25, false);
			}
		}
	}
}

void bunnyHOP() {
	uintptr_t localPlayer = driver_interface::read_memory<uintptr_t>(process_id, client_base + hazedumper::signatures::dwLocalPlayer);
	uintptr_t is_jumping = driver_interface::read_memory<uintptr_t>(process_id, localPlayer + hazedumper::netvars::m_fFlags);

	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (is_jumping & (1 << 0))) {
		driver_interface::write_memory<uintptr_t>(process_id, (client_base + hazedumper::signatures::dwForceJump), 6);
	}
}