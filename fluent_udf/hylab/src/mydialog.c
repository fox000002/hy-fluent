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
#include "resource.h"

#include <windows.h>

#include "mydialog.h"

#include "myudf.h"

#define IDI_HYLAB  1

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
        {
        SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)LoadIcon(g_hModule, MAKEINTRESOURCE(IDI_HYLAB)));
        }
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

BOOL CALLBACK InfoDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int my_int;
    double my_real;
    char *my_text;
    
    char tmp[128];

    switch (message)
    {
    case WM_INITDIALOG:
        {  
        my_int = rp_get_var_int("my/int");
        my_real = rp_get_var_real("my/real");
        my_text = rp_get_var_string("my/text");      
        
        SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)LoadIcon(g_hModule, MAKEINTRESOURCE(IDI_HYLAB)));
        
        sprintf(tmp, "%g", my_real);
        SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT2), tmp);
        
        SetDlgItemInt(hwndDlg, IDC_EDIT1, my_int, TRUE);
        
        SetDlgItemText(hwndDlg, IDC_EDIT3, my_text);
        }
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            {
                my_int = GetDlgItemInt(hwndDlg, IDC_EDIT1, NULL, TRUE);
                GetDlgItemText(hwndDlg, IDC_EDIT2, tmp, 128);
                my_real = atof(tmp);

                GetDlgItemText(hwndDlg, IDC_EDIT3, tmp, 128);
                my_text = tmp;

                rp_set_var_int("my/int", my_int);
                rp_set_var_real("my/real", my_real);
                rp_set_var_string("my/text", my_text);
                
                return FALSE;
            }
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

/**
 * @brief Show Info Dialog
 *
 */
void showInfoDialog()
{
    /*MessageBox(NULL, msg, "Info", MB_OK);*/
    DialogBox( g_hModule,
               MAKEINTRESOURCE(5001),
               GetForegroundWindow(),
               InfoDialogProc);
}
