#include "public.h"
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

extern HWND     g_hMainWnd= NULL;
static HBITMAP  m_hBkBmp  = NULL;

UINT  m_nMainDlgID=0,m_nCalendarDlgID=0;
UINT  m_nCalendarMenuID=0,m_nExitMenuID=0;
UINT  m_nMenuID = 0;
UINT  m_nBtnCalendarID= 0;
UINT  m_nBtnExitID    = 0;
UINT  m_nSkin = 0; 

INT_PTR CALLBACK ClockDlgProc(HWND hDlg,
							  UINT uMsg,
							  WPARAM wParam,
							  LPARAM lParam
						   );
void SetDlgMenuID(UINT nMainDlgID,UINT nCalendarDlgID,UINT nMenuID)
{
	m_nMainDlgID     = nMainDlgID;
	m_nCalendarDlgID = nCalendarDlgID;
	m_nMenuID = nMenuID;
}
void SetButtonID(UINT nCalendarID,UINT nExitID,UINT nSkin)
{
    m_nBtnCalendarID = nCalendarID;
	m_nBtnExitID = nExitID;
	m_nSkin = nSkin;
}
INT_PTR ShowMainDlg()
{
	return DialogBox(g_hInst,MAKEINTRESOURCE(m_nMainDlgID),0,ClockDlgProc);
}
void UpdateClock(HWND hWnd,HDC hDC)
{
	SYSTEMTIME DateTime;
	HDC hdcMemory=CreateCompatibleDC(hDC);

	//绘制窗体背景图片
	SelectObject(hdcMemory,m_hBkBmp);

	//设置文字背景为透明模式
	SetBkMode(hdcMemory,TRANSPARENT);

	//测试文字输出
	SetTextColor(hdcMemory,RGB(255,255,255));//文字颜色
	//TextOut(hdcMemory,50,50,"2001",4);//文字输出
    

	DrawClockText(hdcMemory);


	BitBlt(hDC,0,0,128,128,hdcMemory,0,0,SRCCOPY);
	DeleteDC(hdcMemory);
}
INT_PTR CALLBACK ClockDlgProc(HWND hDlg,
						   UINT uMsg,
						   WPARAM wParam,
						   LPARAM lParam
						   )
{ SYSTEMTIME st;
   GetLocalTime(&st);
	switch(uMsg)
	{
	case WM_INITDIALOG:
		g_hMainWnd=hDlg;
		SetWindowPos(hDlg, HWND_TOPMOST,0,0,128,128,SWP_NOMOVE);  
		SetWindowText(hDlg,"万年历时钟");
		//SendMessage(hDlg,WM_SETICON,ICON_SMALL,(LPARAM)(LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1))));
		//SendMessage(hDlg,WM_SETICON,ICON_BIG,(LPARAM)(LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1))));
		//添加层风格
		SetWindowLong(hDlg,GWL_EXSTYLE,GetWindowLong(hDlg,GWL_EXSTYLE)|WS_EX_LAYERED);
		//红的地方为透明色
		SetLayeredWindowAttributes(hDlg,RGB(255,255,255),0,LWA_COLORKEY);

		m_hBkBmp = (HBITMAP)LoadImage(g_hInst,
			"../Image/skin/a1.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		
		SetTimer(hDlg,1,500,NULL);
		PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;
		break;
	case WM_TIMER:
		{
			HDC hdc= GetDC(hDlg);
			if((int)st.wMinute==30)
			     PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;
			UpdateClock(hDlg,hdc);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg,&ps);
			UpdateClock(hDlg,hdc);
			EndPaint(hdc,&ps);
		}
		break;
	case WM_LBUTTONDOWN:
		PostMessage(hDlg,WM_NCLBUTTONDOWN,HTCAPTION,lParam); 
		break;
	case WM_RBUTTONUP:
		{
			HMENU hParentMenu,hSubMenu;

			POINT pt={LOWORD(lParam),HIWORD(lParam)};
			ClientToScreen(hDlg,&pt);

		    hParentMenu = LoadMenu(g_hInst,MAKEINTRESOURCE(m_nMenuID));
			hSubMenu=GetSubMenu(hParentMenu,0);
			TrackPopupMenu(hSubMenu,TPM_LEFTALIGN,pt.x,pt.y,0,hDlg,NULL);
		}
		break;
	case WM_COMMAND:
		{			
			if(LOWORD(wParam)==m_nBtnExitID)
			{
				if(HIWORD(wParam)==BN_CLICKED)
					EndDialog(hDlg,0);
			}
			else if(LOWORD(wParam)==m_nBtnCalendarID)
			{
				if(HIWORD(wParam)==BN_CLICKED)
					ShowCalendarDlg(m_nCalendarDlgID);
			}
			/*else if(LOWORD(wParam)==m_nSkinID)
			{
				if(HIWORD(wParam)==BN_CLICKED)
					ShowSetupDlg();
			}*/
		}
		return FALSE;//处理WM_COMMAND消息后，应当返回0
	case WM_CLOSE:
		{
			KillTimer(hDlg,1);
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

