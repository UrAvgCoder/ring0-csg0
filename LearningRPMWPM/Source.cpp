#include <ntifs.h>
#include "Header.hpp"
#include "communication.hpp"

UNICODE_STRING deviceName, SymLink;

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
    UNREFERENCED_PARAMETER(pDriverObject);
    DbgPrintEx(0, 0, "driver unloaded\n");
    IoDeleteDevice(pDriverObject->DeviceObject);
    IoDeleteSymbolicLink(&SymLink);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
    UNREFERENCED_PARAMETER(pRegistryPath);
    pDriverObject->DriverUnload = &DriverUnload;
    DbgPrintEx(0, 0, "driver loaded\n");

    RtlInitUnicodeString(&deviceName, L"\\Device\\pavan");
    RtlInitUnicodeString(&SymLink, L"\\??\\pavanLink");

    NTSTATUS status = 0;
    PDEVICE_OBJECT deviceObj = NULL;
    status = IoCreateDevice(pDriverObject, 0, &deviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &deviceObj);
    if (!NT_SUCCESS(status)) {
        DbgPrintEx(0, 0, "device creation failed: %d\n", status);
        return status;
    }

    status = IoCreateSymbolicLink(&SymLink, &deviceName);
    if (!NT_SUCCESS(status)) {
        DbgPrintEx(0, 0, "device creation failed: %d\n", status);
        return status;
    }

    pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
    pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoAction;

    deviceObj->Flags |= DO_DIRECT_IO;
    deviceObj->Flags &= ~DO_DEVICE_INITIALIZING;

    DbgPrintEx(0, 0, "driver entry complete...\n");

    return STATUS_SUCCESS;
}
