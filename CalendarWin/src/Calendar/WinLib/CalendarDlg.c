#include "public.h"

void UpdateCalendar(HWND hWnd,HDC hDC)
{
	RECT rc;
	HDC hdcMemory;
	HBITMAP hBmp;
	HBRUSH  hBrush;

	GetClientRect(hWnd,&rc);
	hdcMemory=CreateCompatibleDC(hDC);
	hBmp = CreateCompatibleBitmap(hDC,rc.right,rc.bottom);
	SelectObject(hdcMemory,hBmp);

	//���ƴ��屳��
	hBrush=CreateSolidBrush(RGB(255,255,255));
	SelectObject(hdcMemory,hBrush);
	Rectangle(hdcMemory,0,0,rc.right,rc.bottom);

	//�������ֱ���Ϊ͸��ģʽ
	SetBkMode(hdcMemory,TRANSPARENT);

	//
	DrawCalendar(hdcMemory,rc.right,rc.bottom);

	BitBlt(hDC,0,0,rc.right,rc.bottom,hdcMemory,0,0,SRCCOPY);
	DeleteObject(hBrush);
	DeleteObject(hBmp);
	DeleteDC(hdcMemory);

}
INT_PTR CALLBACK CalendarDlgProc(HWND hDlg,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam
							)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		//SetWindowPos(hDlg, HWND_TOPMOST,600,200,128,128,SWP_SHOWWINDOW);  
		//SetWindowText(hDlg,"������ʱ��");
		break;
	case WM_TIMER:
		{
			HDC hdc= GetDC(hDlg);
			UpdateCalendar(hDlg,hdc);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg,&ps);
			UpdateCalendar(hDlg,hdc);
			EndPath(hdc);
		}
		break;
	case WM_LBUTTONDOWN:
		PostMessage(hDlg,WM_NCLBUTTONDOWN,HTCAPTION,lParam); 
		break;
	case WM_COMMAND:
		{
			
			if(HIWORD(wParam)==BN_CLICKED)
			{
				if(LOWORD(wParam)==IDCANCEL)
					EndDialog(hDlg,0);
				else
				{
                    CalendarButtonEvent(LOWORD(wParam));
					InvalidateRect(hDlg,NULL,FALSE);//������ʾ
				}
			}
			
		}
		return FALSE;//����WM_COMMAND��Ϣ��Ӧ������0
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR ShowCalendarDlg(UINT nIDDlg)
{
	return DialogBox(g_hInst,MAKEINTRESOURCE(nIDDlg),
		g_hMainWnd,CalendarDlgProc);
}