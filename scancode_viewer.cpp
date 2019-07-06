#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <Windowsx.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <array>
#include <sstream>

// edit_hwnd is a read-only Edit Window that will be used to display the pressed key scancode.
HWND edit_hwnd = nullptr;

int const main_window_width = 150;
int const main_window_height = 75;
int const edit_window_width = 75;
int const edit_window_height = 25;

int16_t scanCodeFromlParam(LPARAM l_param)
{
    int16_t scan_code = 0x0000;
    if ((l_param >> 24) & 0x1)
    {
        scan_code += 0xE000;
    }
    scan_code += (l_param >> 16) & 0xFF;
    return scan_code;
}

LRESULT mainWindowProc(
    HWND hwnd,
    UINT message,
    WPARAM w_param,
    LPARAM l_param)
{
    if (message == WM_DESTROY)
    {
        PostQuitMessage(0);
    }
    if (message == WM_KEYDOWN)
    {
        auto scan_code = scanCodeFromlParam(l_param);
        std::wstringstream sstream;
        sstream
            << TEXT("0x")
            << std::hex
            << std::setw(4)
            << std::setfill(TEXT('0'))
            << scan_code;
        Edit_SetText(edit_hwnd, sstream.str().c_str());
    }
    if (message == WM_PAINT || message == WM_SIZE)
    {
        RECT client_rect;
        if (GetClientRect(hwnd, &client_rect))
        {
            // Keep the Edit window centered in the Main window's client rect.
            SetWindowPos(
                edit_hwnd,
                HWND_TOP,
                (client_rect.right - client_rect.left - edit_window_width) / 2,
                (client_rect.bottom - client_rect.top - edit_window_height) / 2,
                edit_window_width,
                edit_window_height,
                0 // No flags
            );

            PAINTSTRUCT paint_struct;
            BeginPaint(hwnd, &paint_struct);
            FillRect(paint_struct.hdc, &client_rect, CreateSolidBrush(RGB(255, 255, 255)));
            EndPaint(hwnd, &paint_struct);
        }
    }
    return DefWindowProc(hwnd, message, w_param, l_param);
}

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE, PWSTR, int)
{
    // Register the window class for the main window.
    WNDCLASS wc = {};
    wc.lpfnWndProc = (WNDPROC)mainWindowProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = TEXT("MainWndClass");

    if (!RegisterClass(&wc))
    {
        return FALSE;
    }

    HWND main_hwnd = CreateWindowEx(
        0,
        TEXT("MainWndClass"),
        TEXT("Key Scancode viewer"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        main_window_width,
        main_window_height,
        nullptr,
        nullptr,
        hinstance,
        nullptr);

    edit_hwnd = CreateWindowEx(
        0,
        TEXT("EDIT"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_READONLY,
        (main_window_width - edit_window_width) / 2,
        main_window_height / 2,
        edit_window_width,
        edit_window_height,
        main_hwnd,
        nullptr,
        hinstance,
        nullptr);

    ShowWindow(main_hwnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}