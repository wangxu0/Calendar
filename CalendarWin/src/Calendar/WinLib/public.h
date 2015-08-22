/************************************************************************/
/*文件名：public.h                                                      */
/*作  用：全局公共头文件                                                */
/************************************************************************/
#pragma once

#define _WIN32_WINNT 0x0500

#include <Windows.h>
#include <tchar.h>
#include "CalendarDlg.h"
#include "ClockDlg.h"
#include "SetupDlg.h"

//声名'用于保存应用程序实例的句柄'的变量
extern HINSTANCE g_hInst;
//声名‘保存主窗口句柄’的变量
extern HWND      g_hMainWnd;


