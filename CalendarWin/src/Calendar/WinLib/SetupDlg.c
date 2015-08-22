#include "public.h"

INT_PTR CALLBACK SetupDlgProc(HWND hDlg,
								 UINT uMsg,
								 WPARAM wParam,
								 LPARAM lParam
								 )
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		{
			if(HIWORD(wParam)==BN_CLICKED)
			{
				if(LOWORD(wParam)==IDCANCEL)
					EndDialog(hDlg,0);
			}
		}
		return FALSE;//处理WM_COMMAND消息后，应当返回0
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR ShowSetupDlg()
{
	/*return DialogBox(g_hInst,MAKEINTRESOURCE(IDD_DLG_SETUP),
		g_hMainWnd,SetupDlgProc);*/
}