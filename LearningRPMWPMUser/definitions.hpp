#pragma once
#include <Windows.h>
#include <iostream>
#include <array>

#define GET_BASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define READ_REQ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define WRITE_REQ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define WRITE_STRUCT_REQ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

//typedef struct _GLOW_STRUCT_ {
//	BYTE base[4];
//	float red, green, blue, alpha;
//	BYTE buff[16];
//	bool renderwhenocculed, renderwhenunocculed, fullbloom;
//	BYTE buff2[5];
//	int glowStyle;
//} SGlowStruct, * pGlowStruct;


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

//typedef struct _KERNEL_WRITE_STRUCT_REQUEST
//{
//	ULONG pid;
//	ULONG address;
//	SGlowStruct value;
//	ULONG size;
//}KernelWriteStructRequest, * pKernelWriteStructRequest;

struct driver_request
{
	uint64_t pid;
	uint64_t address;
	uint64_t size;

	uint8_t value[1];
};