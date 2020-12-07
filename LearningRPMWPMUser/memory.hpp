#pragma once

#include "definitions.hpp"
#include "GlowStructure.hpp"
#include <TlHelp32.h>


namespace memory {
	ULONG get_process_id(const char* process_name);
}
