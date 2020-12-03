#pragma once
#include <Windows.h>
#include <iostream>

#define GET_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_GET_BASE {
	ULONG pid;
	ULONG client_base;
} SkernelGetBase, * pkernelGetBase;

typedef struct _KERNEL_READ_REQUEST
{
	ULONG pid;
	ULONG address;
	ULONG response;
	ULONG size;

}KernelReadRequest, * pKernelReadRequest;

typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG pid;
	ULONG address;
	ULONG value;
	ULONG size;

}KernelWriteRequest, * pKernelWriteRequest;