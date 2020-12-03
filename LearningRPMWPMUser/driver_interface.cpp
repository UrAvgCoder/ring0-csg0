#include "driver_interface.hpp"

HANDLE driver_interface::get_device(LPCSTR registryPath)
{
    HANDLE hDevice = CreateFile(registryPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice != INVALID_HANDLE_VALUE) {
        return hDevice;
    }
    return NULL;
}

ULONG driver_interface::get_client_base(ULONG process_id)
{
    SkernelGetBase data = { 0 };
    data.pid = process_id;
    auto bytes = 0UL;
    HANDLE hDevice = get_device("\\\\.\\pavanLink");
    if (DeviceIoControl(hDevice, GET_BASE, &data, sizeof(data), &data, sizeof(data), &bytes, NULL)) {
        return data.client_base;
    }
    return 0;
}
