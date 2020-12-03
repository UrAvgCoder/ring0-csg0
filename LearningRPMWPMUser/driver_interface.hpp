#pragma once

#include "memory.hpp"

namespace driver_interface {

	HANDLE get_device(LPCSTR registryPath);
	ULONG get_client_base(ULONG process_id);


}
