#include "memory.hpp"

ULONG memory::get_module_base(PEPROCESS process, LPCWSTR module_name)
{
    if (!process) { return 0; }
    __try {
        DbgPrintEx(0, 0, "works inside method?\n");
        PPEB32 peb32 = (PPEB32)PsGetProcessWow64Process(process);
        if (!peb32 || !peb32->Ldr) { return 0; }

        for (PLIST_ENTRY32 plist_entry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList.Flink;
            plist_entry != &((PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList;
            plist_entry = (PLIST_ENTRY32)plist_entry->Flink)
        {
            //DbgPrintEx(0, 0, "works inside for?\n");
            PLDR_DATA_TABLE_ENTRY32 pentry = CONTAINING_RECORD(plist_entry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);
            if (wcscmp((PWCH)pentry->BaseDllName.Buffer, module_name) == 0) {
                DbgPrintEx(0, 0, "here too?\n");
                DbgPrintEx(0, 0, "value: %x", pentry->DllBase);
                return pentry->DllBase;

            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {

    }
    return 0;
}

