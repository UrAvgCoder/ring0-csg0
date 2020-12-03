#pragma once
#include <ntifs.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);
VOID DriverUnload(PDRIVER_OBJECT pDriverObject);

