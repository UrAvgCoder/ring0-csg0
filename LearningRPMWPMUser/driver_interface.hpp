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
		//info.value = buffer;
		info.size = sizeof(t);
		memcpy(info.value, &buffer, sizeof(t));
		auto bytes = 0UL;
		HANDLE hDevice = get_device("\\\\.\\pavanLink");
		DeviceIoControl(hDevice, WRITE_REQ, &info, sizeof(info), &info, sizeof(info), &bytes, NULL);
	}

	template<typename T>
	bool request_write(ULONG process_id, ULONG address, const T& data) {
		static_assert(sizeof(T) > 0, "Nothing to write");
		
		constexpr size_t alloc_size = sizeof(driver_request) + sizeof(T) - 1;
		std::array<uint8_t, alloc_size> request_buffer;
		//std::array<uint8_t, alloc_size> test = { 0 };
		const auto request = reinterpret_cast<driver_request*>(request_buffer.data());

		request->address = address;
		request->pid = process_id;
		request->size = sizeof(T);

		memcpy(request->value, &data, sizeof(T));
		HANDLE hDevice = get_device("\\\\.\\pavanLink");
		auto bytes = 0UL;
		if (DeviceIoControl(hDevice, WRITE_STRUCT_REQ, request, sizeof(alloc_size), request, sizeof(alloc_size), &bytes, NULL))
			return true;
		else return false;
		
	}

}
