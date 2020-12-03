#pragma once
#include "definitions.hpp"

namespace memory {
	ULONG get_module_base(PEPROCESS process, LPCWSTR module_name);
}
