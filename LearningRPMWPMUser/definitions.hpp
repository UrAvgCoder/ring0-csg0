#pragma once
#include <Windows.h>
#include <iostream>

#define GET_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define READ_REQ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define WRITE_REQ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _GLOW_STRUCT_ {
	BYTE base[4];
	float red, green, blue, alpha;
	BYTE buff[16];
	bool renderwhenocculed, renderwhenunocculed, fullbloom;
	BYTE buff2[5];
	int glowStyle;
} SGlowStruct, * pGlowStruct;


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

typedef struct _KERNEL_WRITE_STRUCT_REQUEST
{
	ULONG pid;
	ULONG address;
	SGlowStruct value;
	ULONG size;
}KernelWriteStructRequest, * pKernelWriteStructRequest;