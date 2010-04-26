#include <windows.h>

#include "mydialog.h"

HMODULE g_hModule = NULL;

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        /* LOG_STRING("UHook DLL_PROCESS_ATTACH!"); */
        break;
    case DLL_THREAD_ATTACH:
        g_hModule = hModule;
        /* LOG_STRING("UHook DLL_THREAD_ATTACH!"); */
        break;
    case DLL_THREAD_DETACH:
        /* Do thread-specific cleanup.*/
        /* LOG_STRING("UHook DLL_THREAD_DETACH!");*/
        break;

    case DLL_PROCESS_DETACH:
        /* Perform any necessary cleanup.*/
        /*LOG_STRING("UHook DLL_PROCESS_DETACH!");*/
        break;
    }
    return TRUE;

    UNREFERENCED_PARAMETER(lpReserved);
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwndDlg, LOWORD(wParam));
            return TRUE;
        }
    }
    return FALSE;
}

void showMyMessage(char *msg)
{
    /*MessageBox(NULL, msg, "Info", MB_OK);*/
    DialogBox( g_hModule,
               MAKEINTRESOURCE(5000),
               GetForegroundWindow(),
               DialogProc);
}
