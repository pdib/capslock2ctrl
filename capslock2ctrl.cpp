#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <array>
#include <iostream>

HKEY const registry_key = HKEY_LOCAL_MACHINE;
auto const registry_subkey = TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout");
auto const registry_value = TEXT("Scancode Map");
auto const registry_value_full = TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout\\Scancode Map");

void writeToRegistry(int16_t keyFrom, int16_t keyTo)
{
    std::array<BYTE, 100> data;
    unsigned long data_size = data.size();

    HKEY keyboard_layout_key;

    auto status = RegOpenKeyEx(
        registry_key,
        registry_subkey,
        0,
        KEY_ALL_ACCESS,
        &keyboard_layout_key);

    // status = RegGetValue(
    //     keyboard_layout_key,
    //     nullptr,
    //     registry_value,
    //     RRF_RT_REG_BINARY,
    //     nullptr,
    //     static_cast<void *>(data.data()),
    //     &data_size);

    int keyMappingValue = (keyFrom << 16) + keyTo;

    std::array<int32_t, 5> init_data = {0, 0, 2, keyMappingValue, 0};

    status = RegSetValueEx(
        keyboard_layout_key,
        registry_value,
        0,
        REG_BINARY,
        (BYTE *)(init_data.data()),
        init_data.size() * sizeof(int32_t));

    status = RegCloseKey(keyboard_layout_key);
}

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE, PWSTR, int)
{
    int const capslock_scancode = 0x003a;
    int const leftctrl_scancode = 0x001d;
    writeToRegistry(capslock_scancode, leftctrl_scancode);
    return 0;
}