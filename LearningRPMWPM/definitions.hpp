#pragma once

#include "ntapi.hpp"

#define GET_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define READ_REQ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define WRITE_REQ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define WRITE_STRUCT_REQ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_GET_BASE {
	ULONG pid;
	ULONG client_base;
} SkernelGetBase, * pkernelGetBase;

typedef struct _KERNEL_READ_REQUEST
{
	ULONG pid;
	ULONG address;
	UCHAR response[8000];
	ULONG size;

}KernelReadRequest, * pKernelReadRequest;

typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG pid;
	ULONG address;
	UCHAR value[8000];
	ULONG size;

}KernelWriteRequest, * pKernelWriteRequest;

struct driver_request
{
	UINT64 pid;
	UINT64 address;
	UINT64 size;

	UINT8 value[1];
};