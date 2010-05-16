/********************************************************************
    created:    2010/04/28
    created:    28:4:2010   18:18
    filename:     mydialog.c
    file path:    hy-fluent/fluent_udf/hylab/src
    file base:    mydialog
    file ext:    c
    author:        hu

    purpose:    A simple about dialog.
*********************************************************************/

#include <windows.h>

#include "mydialog.h"



/** 
 * @brief 
 */
HMODULE g_hModule = NULL;

/** 
 * @brief DLL entry
 * 
 * @param hModule
 * @param ul_reason_for_call
 * @param lpReserved
 * 
 * @return 
 *    When the system calls the DllMain function with the DLL_PROCESS_ATTACH value,
 *    the function returns TRUE if it succeeds or FALSE if initialization fails. 
 *    When the system calls the DllMain function with any value other than 
 *    DLL_PROCESS_ATTACH, the return value is ignored.
 */
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

/** 
 * @brief About Dialog Procedure
 * 
 * @param hwndDlg
 * @param message
 * @param wParam
 * @param lParam
 * 
 * @return 
 */
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

/** 
 * @brief Show About Dialog
 * 
 * @param msg
 */
void showMyMessage(char *msg)
{
    /*MessageBox(NULL, msg, "Info", MB_OK);*/
    DialogBox( g_hModule,
               MAKEINTRESOURCE(5000),
               GetForegroundWindow(),
               DialogProc);
}
