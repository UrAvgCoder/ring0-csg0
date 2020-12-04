#pragma once
#include "definitions.hpp"

namespace memory {
	ULONG get_module_base(PEPROCESS process, LPCWSTR module_name);

	NTSTATUS read_memory(PEPROCESS target_process, PVOID source, PVOID target, size_t size);

	NTSTATUS write_memory(PEPROCESS target_process, PVOID source, PVOID target, size_t size);
}
