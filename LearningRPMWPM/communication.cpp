#include "communication.hpp"
#include "definitions.hpp"
#include "memory.hpp"

NTSTATUS CreateCall(PDEVICE_OBJECT pOb, PIRP pIRP)
{
	UNREFERENCED_PARAMETER(pOb);
	DbgPrintEx(0, 0, "create call was called from user\n");
	pIRP->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIRP, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS CloseCall(PDEVICE_OBJECT pOb, PIRP pIRP)
{
	DbgPrintEx(0, 0, "close call was called from user\n");
	UNREFERENCED_PARAMETER(pOb);
	pIRP->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIRP, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS IoAction(PDEVICE_OBJECT pOb, PIRP pIRP)
{
	
	UNREFERENCED_PARAMETER(pOb);
	DbgPrintEx(0, 0, "IoAction called\n");

	NTSTATUS status = STATUS_UNSUCCESSFUL;
	auto bytes = 0UL;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIRP);

	if (stack->Parameters.DeviceIoControl.IoControlCode == GET_BASE) {
		pkernelGetBase buffer = (pkernelGetBase)pIRP->AssociatedIrp.SystemBuffer;
		DbgPrintEx(0, 0, "get clientbase IOCTL called\n");
		PEPROCESS target_process = NULL;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)buffer->pid, &target_process))) {
			DbgPrintEx(0, 0, "works here?\n");
			KAPC_STATE apc;
			KeStackAttachProcess(target_process, &apc);
			ULONG base = memory::get_module_base(target_process, L"client.dll");
			KeUnstackDetachProcess(&apc);
			DbgPrintEx(0, 0, "base value in comms: %x", base);
			if (base) {
				buffer->client_base = base;
				status = STATUS_SUCCESS;
				bytes = sizeof(SkernelGetBase);
			}
		}
	}
	else if (stack->Parameters.DeviceIoControl.IoControlCode == READ_REQ) {
		pKernelReadRequest buffer = (pKernelReadRequest)pIRP->AssociatedIrp.SystemBuffer;
		PEPROCESS target_process = NULL;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)buffer->pid, &target_process))) {
			memory::read_memory(target_process, &buffer->address, &buffer->response, buffer->size);
		}
	}
	/*else if (stack->Parameters.DeviceIoControl.IoControlCode == DIFF_CODE) {
		DbgPrintEx(0, 0, "Subtraction called\n");
		buffer->diff = buffer->numberOne - buffer->numberTwo;

		status = STATUS_SUCCESS;
		bytes = sizeof(SkernelSumData);
	}*/

	pIRP->IoStatus.Status = status;
	pIRP->IoStatus.Information = bytes;
	IoCompleteRequest(pIRP, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}
