#include "Header.hpp"
#include "communication.hpp"

UNICODE_STRING deviceName, SymLink, driver_name;

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
    UNREFERENCED_PARAMETER(pDriverObject);
    DbgPrintEx(0, 0, "driver unloaded\n");
    IoDeleteDevice(pDriverObject->DeviceObject);
    IoDeleteSymbolicLink(&SymLink);
}


NTSTATUS DriverEntryTest(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
    UNREFERENCED_PARAMETER(pRegistryPath);
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
    pDriverObject->DriverUnload = NULL;

    deviceObj->Flags |= DO_DIRECT_IO;
    deviceObj->Flags &= ~DO_DEVICE_INITIALIZING;

    DbgPrintEx(0, 0, "driver entry complete...\n");

    return STATUS_SUCCESS;
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{   
    	RtlInitUnicodeString(&driver_name, L"\\Driver\\pavandriver");
    	NTSTATUS status = IoCreateDriver(&driver_name, &DriverEntryTest);
    	return STATUS_SUCCESS;
}
