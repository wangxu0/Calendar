#pragma once

//创建并显示"日历对话框"
INT_PTR ShowCalendarDlg(UINT nIDDlg);

//日历绘制函数，学员自己编写该函数
void DrawCalendar(HDC hDC,int nWidth,int nHeight);
//事件函数：点击日历界面上各个按钮后被调用
void CalendarButtonEvent(UINT nID);