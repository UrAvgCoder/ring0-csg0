#include "memory.hpp"

ULONG memory::get_module_base(PEPROCESS process, LPCWSTR module_name)
{
    if (!process) { return 0; }
    __try {
        PPEB32 peb32 = (PPEB32)PsGetProcessWow64Process(process);
        if (!peb32 || !peb32->Ldr) { return 0; }

        for (PLIST_ENTRY32 plist_entry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList.Flink;
            plist_entry != &((PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList;
            plist_entry = (PLIST_ENTRY32)plist_entry->Flink)
        {
            PLDR_DATA_TABLE_ENTRY32 pentry = CONTAINING_RECORD(plist_entry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);
            if (wcscmp((PWCH)pentry->BaseDllName.Buffer, module_name) == 0) {
                return pentry->DllBase;
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {

    }
    return 0;
}
