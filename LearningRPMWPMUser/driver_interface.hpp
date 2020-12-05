#pragma once

#include "memory.hpp"

namespace driver_interface {

	HANDLE get_device(LPCSTR registryPath);
	ULONG get_client_base(ULONG process_id);

	template<typename t>
	t read_memory(ULONG process_id, ULONG address);

	template<typename t>
	void write_memory(ULONG process_id, ULONG address, t buffer);

	void write_memory_struct(ULONG process_id, ULONG address, SGlowStruct buffer);

	template<typename t>
	t read_memory(ULONG process_id, ULONG address)
	{
		KernelReadRequest info = { 0 };
		info.pid = process_id;
		info.address = address;
		info.size = sizeof(t);
		auto bytes = 0UL;
		HANDLE hDevice = get_device("\\\\.\\pavanLink");
		if (DeviceIoControl(hDevice, READ_REQ, &info, sizeof(info), &info, sizeof(info), &bytes, NULL)) {
			return *(t*)&info.response;
		}
	}

	template<typename t>
	void write_memory(ULONG process_id, ULONG address, t buffer)
	{
		KernelWriteRequest info = { 0 };
		info.pid = process_id;
		info.address = address;
		info.value = buffer;
		info.size = sizeof(t);
		auto bytes = 0UL;
		HANDLE hDevice = get_device("\\\\.\\pavanLink");
		DeviceIoControl(hDevice, WRITE_REQ, &info, sizeof(info), &info, sizeof(info), &bytes, NULL);
	}


}
