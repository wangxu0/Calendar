#include <Windows.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "resource.h"
#pragma comment(lib,"WinLib.lib")
void SetDlgMenuID(UINT nMainDlgID,UINT nCalendarDlgID,UINT nMenuID);
void SetButtonID(UINT nCalendarID,UINT nExitID,UINT nSkinID);
INT_PTR ShowMainDlg();
/* 
*********************************************************
API函数参考：
GetLocalTime: 获得当前时间(公历：年、月、日、周)
MoveToEx:     移动划线的起点
LineTo:       绘制到某一点
SetTextColor: 设置要绘制的文字颜色
TextOut:      绘制文字
SetBkMode:   设置背景显示模式
wsprintf:    整数转换为字符串，宽字符，用Unicode码
GetClientRect:  获取窗口大小
GetDC:   获取内存设备
SetTimer:  设置定时器
FillRect:  填充矩形
CreateSolidBrush:  创建刷子
RGB:   设置RGB颜色
CreateFont:创建一种有特殊性的字体
结构体类型参考：
SYSTEMTIME:   描述时间类型，包括年月日、时分秒、周
*********************************************************
*/

HINSTANCE g_hInst;//保存操作系统传递进来的"应用程序实例句柄"

//Windows程序入口函数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nShowCmd )
{
	g_hInst = hInstance;
	SetDlgMenuID(IDD_DLG_CLOCK,IDD_DLG_CALENDAR,IDR_MENU);
	SetButtonID(ID_CALENDAR,ID_EXIT,ID_SKIN);
	ShowMainDlg();
}

typedef struct _stDateTime
{
	int nYear;
	int nMonth;
	int nDay;
}stDateTime;
//定义表格，存储1901年-2099年的农历信息：每年的大小月、闰月信息、春节所在的公历日期
unsigned int LunarCalendarTable[199] = 
{
	0x04AE53,0x0A5748,0x5526BD,0x0D2650,0x0D9544,0x46AAB9,0x056A4D,0x09AD42,0x24AEB6,0x04AE4A,/*1901-1910*/
	0x6A4DBE,0x0A4D52,0x0D2546,0x5D52BA,0x0B544E,0x0D6A43,0x296D37,0x095B4B,0x749BC1,0x049754,/*1911-1920*/
	0x0A4B48,0x5B25BC,0x06A550,0x06D445,0x4ADAB8,0x02B64D,0x095742,0x2497B7,0x04974A,0x664B3E,/*1921-1930*/
	0x0D4A51,0x0EA546,0x56D4BA,0x05AD4E,0x02B644,0x393738,0x092E4B,0x7C96BF,0x0C9553,0x0D4A48,/*1931-1940*/   
	0x6DA53B,0x0B554F,0x056A45,0x4AADB9,0x025D4D,0x092D42,0x2C95B6,0x0A954A,0x7B4ABD,0x06CA51,/*1941-1950*/
	0x0B5546,0x555ABB,0x04DA4E,0x0A5B43,0x352BB8,0x052B4C,0x8A953F,0x0E9552,0x06AA48,0x6AD53C,/*1951-1960*/
	0x0AB54F,0x04B645,0x4A5739,0x0A574D,0x052642,0x3E9335,0x0D9549,0x75AABE,0x056A51,0x096D46,/*1961-1970*/
	0x54AEBB,0x04AD4F,0x0A4D43,0x4D26B7,0x0D254B,0x8D52BF,0x0B5452,0x0B6A47,0x696D3C,0x095B50,/*1971-1980*/
	0x049B45,0x4A4BB9,0x0A4B4D,0xAB25C2,0x06A554,0x06D449,0x6ADA3D,0x0AB651,0x093746,0x5497BB,/*1981-1990*/
	0x04974F,0x064B44,0x36A537,0x0EA54A,0x86B2BF,0x05AC53,0x0AB647,0x5936BC,0x092E50,0x0C9645,/*1991-2000*/
	0x4D4AB8,0x0D4A4C,0x0DA541,0x25AAB6,0x056A49,0x7AADBD,0x025D52,0x092D47,0x5C95BA,0x0A954E,/*2001-2010*/
	0x0B4A43,0x4B5537,0x0AD54A,0x955ABF,0x04BA53,0x0A5B48,0x652BBC,0x052B50,0x0A9345,0x474AB9,/*2011-2020*/
	0x06AA4C,0x0AD541,0x24DAB6,0x04B64A,0x69573D,0x0A4E51,0x0D2646,0x5E933A,0x0D534D,0x05AA43,/*2021-2030*/
	0x36B537,0x096D4B,0xB4AEBF,0x04AD53,0x0A4D48,0x6D25BC,0x0D254F,0x0D5244,0x5DAA38,0x0B5A4C,/*2031-2040*/
	0x056D41,0x24ADB6,0x049B4A,0x7A4BBE,0x0A4B51,0x0AA546,0x5B52BA,0x06D24E,0x0ADA42,0x355B37,/*2041-2050*/
	0x09374B,0x8497C1,0x049753,0x064B48,0x66A53C,0x0EA54F,0x06B244,0x4AB638,0x0AAE4C,0x092E42,/*2051-2060*/
	0x3C9735,0x0C9649,0x7D4ABD,0x0D4A51,0x0DA545,0x55AABA,0x056A4E,0x0A6D43,0x452EB7,0x052D4B,/*2061-2070*/
	0x8A95BF,0x0A9553,0x0B4A47,0x6B553B,0x0AD54F,0x055A45,0x4A5D38,0x0A5B4C,0x052B42,0x3A93B6,/*2071-2080*/
	0x069349,0x7729BD,0x06AA51,0x0AD546,0x54DABA,0x04B64E,0x0A5743,0x452738,0x0D264A,0x8E933E,/*2081-2090*/
	0x0D5252,0x0DAA47,0x66B53B,0x056D4F,0x04AE45,0x4A4EB9,0x0A4D4C,0x0D1541,0x2D92B5          /*2091-2099*/
};

char* TianGanDizhi[60]=
{
	"甲子","乙丑","丙寅","丁卯","戊辰","已巳","庚午","辛未","壬申","癸酉",
	"甲戌","乙亥","丙子","丁丑","戊寅","已卯","庚辰","辛巳","壬午","癸未",
	"甲申","乙酉","丙戌","丁亥","戊子","已丑","庚寅","辛卯","壬辰","癸巳",
	"甲午","乙未","丙申","丁酉","戊戌","已亥","庚子","辛丑","壬寅","癸卯",
	"甲辰","乙巳","丙午","丁未","戊申","已酉","庚戌","辛亥","壬子","癸丑",
	"甲寅","乙卯","丙辰","丁巳","戊午","已未","庚申","辛酉","壬戌","癸亥 "
};
char *ChDayTable[] = 
{
	"*","初一","初二","初三","初四","初五",
	"初六","初七","初八","初九","初十",
	"十一","十二","十三","十四","十五",
	"十六","十七","十八","十九","二十",
	"廿一","廿二","廿三","廿四","廿五",
	"廿六","廿七","廿八","廿九","三十"
};

char *ChMonthTable[] = {"*","正","二","三","四","五","六","七","八","九","十","十一","腊"};

//定义表格，存储每月1日离元旦的天数（不考虑闰年）
unsigned int MonthAdd[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

//定义用户要显示的年、月、日
unsigned int nCurrentYear=0;
unsigned int nCurrentMonth=0;
unsigned int nCurrentDay=0;


//判断某年是否为公历闰年
int IsLeapYear(WORD iYear)   
{
	if(iYear%4==0&&iYear%100!=0)
		return 1;
	if(iYear%400==0)
		return 1;//已经改过了
	return 0;
}

//公历日期转农历日期的函数
void SunToLunar(const stDateTime* pSunDay,stDateTime* pLunarDay)
{
	int i=0;
	int SunDiff=0;
	int SpringDiff=0;

	int nSpringYear=pSunDay->nYear;
	int nSpringMonth=1;
	int nSpringDay=0;

	//计算当前公历日离当年元旦的天数
	SunDiff = MonthAdd[pSunDay->nMonth-1] + pSunDay->nDay - 1;
	if(IsLeapYear(pSunDay->nYear) && (pSunDay->nMonth > 2) )//若是瑞年且大于2月，天数再加1
		SunDiff++;

	//计算春节所在公历日离元旦的天数：通过查表，计算当年的春节所在的公历日期，
	if(((LunarCalendarTable[pSunDay->nYear - 1901] & 0x60)>>5)==1)//若春节在公历1月
		SpringDiff= (LunarCalendarTable[pSunDay->nYear-1901]&0x1F)-1;
	else//若春节在公历2月
		SpringDiff= (LunarCalendarTable[pSunDay->nYear-1901]&0x1F)-1+31;

	//SunDiff和SpringDiff的差就是当前公历日离春节的天数

	if(SunDiff>=SpringDiff)//公历日在春节后
	{
		int x = SunDiff - SpringDiff;
		int bLunar = (LunarCalendarTable[nSpringYear - 1901] & 0xF00000)>>20;//闰月月份

		i=0;
		nSpringMonth=1;

		for(;;)
		{
			int a = (LunarCalendarTable[pSunDay->nYear-1901] & (0x80000>>i));//计算大小月
			if(a==0)
				a=29;
			else
				a=30;
			if(x<a)
				break;
			x =x - a;
			i++;
			nSpringMonth++;
		}
		nSpringDay =x+1;
		//如果有闰月
		if(nSpringMonth>bLunar && bLunar>0)
			nSpringMonth--;
	}
	else//公历日在春节前
	{

		int x = SpringDiff-SunDiff;
		int nShift=0;

		int bLunar = (LunarCalendarTable[nSpringYear - 1901] & 0xF00000)>>20;//闰月月份

		i=0;
		nSpringYear--;//需要上一年的农历信息
		nSpringMonth=12;//从最后一个月开始计算

		if (bLunar == 0)//如果没有润月
			nShift = 0x100;
		else
			nShift = 0x80;

		for(;;)
		{
			int a = (LunarCalendarTable[nSpringYear-1901] & (nShift<<i));//计算大小月
			if(a==0)
				a=29;
			else
				a=30;

			if(x<a)
			{
				x = a-x;
				break;
			}
			x =x - a;
			i++;
			nSpringMonth--;
		}
		nSpringDay =x+1;

		if(nSpringMonth<bLunar && bLunar>0)
			nSpringMonth++;
	}
	pLunarDay->nYear = nSpringYear;
	pLunarDay->nMonth= nSpringMonth;
	pLunarDay->nDay  = nSpringDay;
}
//根据公历日期获得星期
int GetDayOfWeek(const stDateTime* pSunDay)
{
	//根据泰勒公式
	//nWeek = [nYear1÷4] - 2*nYear1 + nYear2 + [nYear2÷4] + [26(nMonth+1)÷10] + nDay - 1
	int nWeek=0;
	int nYear1=pSunDay->nYear/100;//年的前两位数字
	int nYear2=pSunDay->nYear%100;//年的后两位数字
	int nDay=pSunDay->nDay;  //公立日

	int nMonth=pSunDay->nMonth;//公立月数，所求的月份如果是1月或2月，
	//则应视为上一年的13月或14月，
	//也就是公式中nMonth的取值范围是从3到14而不是从1到12

	if(nMonth==1 || nMonth==2)
	{
		nMonth=12+nMonth;
		nYear1=(pSunDay->nYear-1)/100;//年的前两位数字
		nYear2=(pSunDay->nYear-1)%100;//年的后两位数字
	}

	nWeek = nYear1/4 - 2*nYear1 + nYear2 + nYear2/4 + (26*(nMonth+1))/10 + nDay - 1;
	if(nWeek < 0)
		nWeek = ((nWeek)%7+7);//     
	else
		nWeek = (nWeek)%7;
	return nWeek;
}



//根据公立年得到农历干支年
char* GetGanZhi(int nYear)
{
	return TianGanDizhi[nYear%60-3-1];
}

//日历绘制函数
int count=1;
void DrawCalendar(HDC hDC,int nWidth,int nHeight)
{
    SYSTEMTIME DateTime;//定义时间结构体变量
    stDateTime SunDay,LunarDay,sundate_today;//阳历、阴历、今天的阳历日期
	RECT		rc;
	HBRUSH hBrush;//刷子
	HFONT   hFont;
	HBITMAP hbmp=NULL;
	HBITMAP m_hB2;//贴图
	HBITMAP hBkmap;
	HDC hdc;//设备句柄
	HPEN hpen;  //笔
	const int xRight  = nWidth - 50;
	const int yBottom = nHeight- 10;
	const char *week[]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
	const char *weekEng[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    char str[3],tt[20],ttt[100],tttt[100],str1[3];
	int days=0,Height=0,Width=0,k=0, w=0,i;

   

//贴背景图片。
	hdc = CreateCompatibleDC(hDC);//该函数创建一个与指定设备兼容的内存设备上下文环境(DC)
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/skin/a5.bmp",IMAGE_BITMAP,545,430,LR_LOADFROMFILE);
    SelectObject(hdc,hBkmap);
	BitBlt(hDC,0,0,545,430,hdc,0,0,SRCCOPY);
	   

//填充整个日历的矩形，添加背景颜色。、、  换成贴图。
	//rc.left=0;
	//rc.top=0;
	//rc.bottom=450;
	//rc.right=550;
	//hBrush=CreateSolidBrush(RGB(0,255,0));
	//FillRect(hDC,&rc,hBrush); //填充矩形

//获取当前时间
	GetLocalTime(&DateTime);
	if(nCurrentYear==0)
	{
        nCurrentYear = DateTime.wYear;
		nCurrentMonth= DateTime.wMonth;
		nCurrentDay =DateTime.wDay;
	}
	SunDay.nYear  =  nCurrentYear;
	SunDay.nMonth =  nCurrentMonth;
	SunDay.nDay   =  nCurrentDay;
	sundate_today.nDay=1;
	sundate_today.nYear=nCurrentYear;
	sundate_today.nMonth=nCurrentMonth;


//输出标题
	
		hFont=CreateFont(20,20,0,0,FW_REGULAR,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS, "字体的输出格式");    //字体输出格式 创建一种有特殊性的逻辑字体
        SelectObject(hDC,hFont);
	    SetTextColor(hDC,RGB(255,70,70));
	    wsprintf(tttt,"风调雨顺");
		TextOut(hDC,30,5,tttt,strlen(tttt));

		hFont=CreateFont(20,20,0,0,FW_REGULAR,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS, "字体的输出格式");    //字体输出格式 创建一种有特殊性的逻辑字体
        SelectObject(hDC,hFont);
	    SetTextColor(hDC,RGB(255,70,70));
        wsprintf(tttt,"岁岁平安");
        TextOut(hDC,28,25,tttt,strlen(tttt)); 
	
	   
//在右上角输出年月
	hFont=CreateFont(14,12,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,"字体的输出格式");
    SelectObject(hDC,hFont);
	SetTextColor(hDC,RGB(255,0,0));
    wsprintf(ttt,"%.4d年  %.2d月",SunDay.nYear,SunDay.nMonth);
	TextOut(hDC,270,25,ttt,strlen(ttt));
	 
    

////绘制日历表的格式框架
//	rc.left=21;//填充输出星期的矩形
//	rc.top=55;
//	rc.bottom=105;
//	rc.right=489;
//	hBrush=CreateSolidBrush(RGB(0,255,0));
//	FillRect(hDC,&rc,hBrush); //填充矩形


//绘制表格
	for(i = 55; i < 110; i += 25)//画横线，上下范围
	{
		MoveToEx (hDC, 20, i, NULL);//x轴的最左端
		LineTo (hDC, 489, i);//x轴的最右端
	}
    for(i = 105; i < 450; i += 50)//画横线，上下范围
	{
		MoveToEx (hDC, 20, i, NULL);//x轴的最左端
		LineTo (hDC, 489, i);//x轴的最右端
	}
    for(i = 20; i <= 489; i += 67)
	{
        MoveToEx (hDC, i, 55, NULL);//y轴的最上端
		LineTo (hDC, i, 405);//y轴的最下端
	}


//在表格下方输出农历年信息
	hFont=CreateFont(14,12,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,"字体的输出格式");
    SelectObject(hDC,hFont);
	SetTextColor(hDC,RGB(0,0,0));
	switch(SunDay.nYear%12)
	{
	case 0:
	       TextOut(hDC,122,410,"猴年",4);break;
	case 1:
		   TextOut(hDC,122,410,"鸡年",4);break;
	case 2:
		   TextOut(hDC,122,410,"狗年",4);break;
	case 3:
		   TextOut(hDC,122,410,"猪年",4);break;
	case 4:
		   TextOut(hDC,122,410,"鼠年",4);break;
	case 5:
		   TextOut(hDC,122,410,"牛年",4);break;
    case 6:
		   TextOut(hDC,122,410,"虎年",4);break;
    case 7:
		   TextOut(hDC,122,410,"兔年",4);break;
    case 8:
	       TextOut(hDC,122,410,"龙年",4);break;     
	case 9:
		   TextOut(hDC,122,410,"蛇年",4);break;
	case 10:
		   TextOut(hDC,122,410,"马年",4);break;
	case 11:
		   TextOut(hDC,122,410,"羊年",4);break;//把字符改成ASCII码为什么不行呢？？？？？
	}
	TextOut(hDC,30,410,"农历",4);
	TextOut(hDC,76,410,GetGanZhi(SunDay.nYear),4);//调用不了，需要修改。//改正完毕！！！//需要调色~~
	//TextOut(hDC,106,360,)
   
//生肖贴图
	hdc = CreateCompatibleDC(hDC);
	switch((nCurrentYear-4)%12)
	{
	case 0:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/0.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 1:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/1.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 2:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/2.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 3:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/3.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 4:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/4.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 5:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/5.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 6:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/6.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 7:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/7.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 8:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../image/shengxiao/8.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 9:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/9.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 10:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/10.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	case 11:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/11.bmp",IMAGE_BITMAP,20,20,LR_LOADFROMFILE);
		break;
	}
	SelectObject(hdc,hBkmap);
	BitBlt(hDC,180,410,20,20,hdc,0,0,SRCCOPY);

	


//输出框架上方的星期
    hFont=CreateFont(11,8,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,"设置字体颜色");
    SelectObject(hDC,hFont);
	SetTextColor(hDC,RGB(0,255,0));//设计汉字颜色，输出星期一到星期五
	for(i=1;i<6;i++)
	{
		TextOut(hDC,30+67*i,60,week[i],6);
		TextOut(hDC,30+67*i,85,weekEng[i],3);
	}
    SetTextColor(hDC,RGB(255,0,0));//设为红色
	TextOut(hDC,30,60,week[0],6); 
	TextOut(hDC,30,85,weekEng[0],3);
    TextOut(hDC,440,60,week[6],6);
	TextOut(hDC,440,85,weekEng[6],3);//设计“星期日”和“星期六”位置及颜色


//输出日期
    	SunToLunar(&SunDay,&LunarDay);//获得当前阳历历转阴历日期
	if(SunDay.nMonth ==1||SunDay.nMonth ==3||SunDay.nMonth ==5||SunDay.nMonth ==7||SunDay.nMonth ==8||SunDay.nMonth ==10||SunDay.nMonth ==12)
		days=31;
	else if(SunDay.nMonth ==4||SunDay.nMonth ==6||SunDay.nMonth ==9||SunDay.nMonth ==11)
		days=30;
	else if(SunDay.nMonth ==2&&IsLeapYear(SunDay.nYear))
	    days=29;
	else
		days=28;
	    w=GetDayOfWeek(&sundate_today);//根据公历日期获得星期
		for(i=1;i<=days;i++)
	{   
		SunDay.nDay=i;
	    SunToLunar(&SunDay,&LunarDay);//获得公历日期转农历日期
		hFont=CreateFont(15,10,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,"设置字体格式");//设置字体和大小
        SelectObject(hDC,hFont);//选择你获得的刷子
		Width=(w+i-1)%7;
	    Height=((w+i-1)-(w+i-1)%7)/7;
//填充当天日期的背景颜色，起标示和防止覆盖的作用。
		if(i==DateTime.wDay && nCurrentYear==DateTime.wYear && nCurrentMonth==DateTime.wMonth)
		{
			rc.left=20+(Width)*67;
	        rc.top=105+(Height)*50;
	        rc.right=20+(Width+1)*67;
	        rc.bottom=105+(Height+1)*50;
			hBrush=CreateSolidBrush(RGB(255,10,0));
	        FillRect(hDC,&rc,hBrush);
	    }


		

//分别输出阴历和阳历日期
		
			wsprintf(str,"%d",i);
		    SetTextColor(hDC,RGB(0,0,0));
		    TextOut(hDC,30+(Width)*70,110+Height*50,str,strlen(str));  //阳历
			TextOut(hDC,20+(Width)*70,135+Height*50,ChDayTable[LunarDay.nDay],4);//阴历



//标示出节日
			hFont=CreateFont(14,9,0,0,0,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,DEFAULT_PITCH | FF_MODERN, "设置字体格式");    //字体输出格式
	      SelectObject(hDC,hFont);
		  SetTextColor(hDC,RGB(255,50,247));//设定输出节日的颜色
         if (LunarDay.nMonth==1&&LunarDay.nDay==1)
		 {
			 wsprintf(tt,"春节");//把字符改成ＡＳＣＩＩ码试试》》》》
			 TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 } 
		 if (LunarDay.nMonth==1&&LunarDay.nDay==15)
		 {   
			 wsprintf(tt,"元宵节");
		     TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		   if (SunDay.nMonth==4&&SunDay.nDay==5)
		 {
			  wsprintf(tt,"清明节");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if (LunarDay.nMonth==5&&LunarDay.nDay==5)
		 {
			 wsprintf(tt,"端午节"); 
			 TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
      		  if (LunarDay.nMonth==8&&LunarDay.nDay==15)
		 {
			  wsprintf(tt,"中秋节");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==1&&SunDay.nDay==1)
		 {
			  wsprintf(tt,"元旦");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==2&&SunDay.nDay==14)
		 {
			  wsprintf(tt,"情人节");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==3&&SunDay.nDay==8)
		 {
			  wsprintf(tt,"妇女节");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
	     if (SunDay.nMonth==4&&SunDay.nDay==1)
		 {
			  wsprintf(tt,"愚人节");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
         if(SunDay.nMonth==5&&SunDay.nDay==1)
		 {
		      wsprintf(tt,"劳动节");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==6&&SunDay.nDay==1)
		 {
			  wsprintf(tt,"儿童节");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==10&&SunDay.nDay==1)
		 {
			  wsprintf(tt,"国庆节");             
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==11&&SunDay.nDay==11)
		 {
			  wsprintf(tt,"光棍节");             
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		  if(SunDay.nMonth==12&&SunDay.nDay==25)
		 {
			  wsprintf(tt,"圣诞节");             
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
	}
	
}


//该函数调用完后，系统自动调用DrawCalendar函数
//      因此，可以理解为，该函数和DrawCalendar函数是联动的
void CalendarButtonEvent(UINT nID)     
{
	switch (nID)
	{
	case IDC_BTN_YEARUP://点击了"年↑"按钮	
		nCurrentYear++;
		break;
	case IDC_BTN_YEARDOWN:
		nCurrentYear--;
		break;
	case IDC_BTN_MONTHUP:
		nCurrentMonth++;
		if(nCurrentMonth>12)//自己改正的。
		{
			nCurrentMonth-=12;
			nCurrentYear++;
		}
		break;
	case IDC_BTN_MONTHDOWN:
		nCurrentMonth--;
		if(nCurrentMonth<=0)//自己改正的。
		{
			nCurrentMonth+=12;
			nCurrentYear--;
		}
		break;
	case IDC_BTN_TODAY:
		nCurrentYear=0;
		break;
	}
}


//该函数每0.5秒被调用一次,时钟界面上的文字绘制
void DrawClockText(HDC hDC)
{ // HFont hf;
  // hf=CreateFont();
   SYSTEMTIME datetime;int i;
   /*自己写的*/
   char tttt[100];                                      //输出时间的中间变量。
   char str[50];
   //HFONT newFont;
   HPEN hpen;
   RECT rc;
   HBRUSH hBrush;
   HFONT newFont;
   HBITMAP hBkmap;
   HDC hdc;



  stDateTime sundate,lunardate;
   GetLocalTime(&datetime);
   sundate.nDay=datetime.wDay;//获取当前系统阳历时间。
   sundate.nMonth =datetime.wMonth;
   sundate.nYear=datetime.wYear;
   wsprintf(str,"%d",sundate.nYear);

//角度输出年月日
   newFont=CreateFont(10,9,480,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,  //高度宽度移位与x轴角度
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"宋体");
    SelectObject(hDC,newFont);//选择获得的刷子
    SetTextColor(hDC,RGB(255,0,0));
	TextOut(hDC,15,40,str,4); //字符串对齐x,y基准点，，字符数
    wsprintf(str,"%.2d",sundate.nMonth);

    newFont=CreateFont(10,9,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"宋体");//用CreateFont函数调整角度、文字大小和类型。
    SelectObject(hDC,newFont);//选择获得的刷子
    TextOut(hDC,61,12,str,2);
	 wsprintf(str,"%.2d",sundate.nDay);

    newFont=CreateFont(10,9,-480,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"宋体");
    SelectObject(hDC,newFont);//选择获得的刷子
    TextOut(hDC,95,17,str,2);

//阴历日期的显示。
   SunToLunar(&sundate,&lunardate);
   i=lunardate.nMonth;
   //TextOut(hDC,20,20,ChMonthTable[i],2);//农历日期在什么地方显示？？？？？？
   //TextOut(hDC,30,20,"月",2);
   
//系统时间的显示。
   hpen=(HPEN)CreateFont(19,9,0,0,9,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"宋体");
	SelectObject(hDC,hpen);
	SetTextColor(hDC,RGB(255, 255, 0));
    rc.left=28;               //填充时间的背景以防止覆盖，模糊。
	rc.top=48;   
	rc.bottom=65;
	rc.right=100;
	hBrush=CreateSolidBrush(RGB(12,45,78));  //显示时间的矩形
	FillRect(hDC,&rc,hBrush); //填充矩形

    

	wsprintf(tttt,"%.2d:%.2d:%.2d",datetime.wHour,datetime.wMinute,datetime.wSecond); //按照输出时间的格式把系统时间存在tttt中。
    //system("CLS");//这样刷屏解决不了覆盖问题。
	//clrscr();
	TextOut(hDC,28,48,tttt,strlen(tttt)); 


//填充时间的背景以防止覆盖，模糊。
    rc.left=34;   //左上角x            
	rc.top=70;     //左上角y
	rc.bottom=85;  //右下角y
	rc.right=93;   //右下角x
	hBrush=CreateSolidBrush(RGB(12,45,78));  
	FillRect(hDC,&rc,hBrush); //填充矩形

//输出星期
	newFont=CreateFont(16,8,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"设置字体格式");
    SelectObject(hDC,newFont);
	SetTextColor(hDC,RGB(0,0,255));

	switch(datetime.wDayOfWeek)
	{
	case 0:
		 TextOut(hDC,46,70,"周日",4);break;//调整位置参数，正确输出文字信息。
	case 1:
		 TextOut(hDC,46,70,"周一",4);break;
	case 2:
		 TextOut(hDC,46,70,"周二",4);break;
	case 3:
		 TextOut(hDC,46,70,"周三",4);break;
	case 4:
		 TextOut(hDC,46,70,"周四",4);break;
	case 5:
		 TextOut(hDC,46,70,"周五",4);break;
	case 6:
		 TextOut(hDC,46,70,"周六",4);break;
	}

//输出生肖年
	 newFont=CreateFont(15,7,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
         CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"宋体");
    SelectObject(hDC,newFont);
	SetTextColor(hDC,RGB(0,255,0));
	 switch(sundate.nYear%12)
	{
	case 0:
	       TextOut(hDC,47,30,"猴年",4);break;
	case 1:
		   TextOut(hDC,47,30,"鸡年",4);break;
	case 2:
		   TextOut(hDC,47,30,"狗年",4);break;
	case 3:
		   TextOut(hDC,47,30,"猪年",4);break;
	case 4:
		   TextOut(hDC,47,30,"鼠年",4);break;
	case 5:
		   TextOut(hDC,47,30,"牛年",4);break;
    case 6:
		   TextOut(hDC,47,30,"虎年",4);break;
    case 7:
		   TextOut(hDC,47,30,"兔年",4);break;
    case 8:
	       TextOut(hDC,47,30,"龙年",4);break;     
	case 9:
		   TextOut(hDC,47,30,"蛇年",4);break;
	case 10:
		   TextOut(hDC,47,30,"马年",4);break;
	case 11:
		   TextOut(hDC,47,30,"羊年",4);break;
	}	
	TextOut(hDC,47,85,GetGanZhi(datetime.wYear),4);


//时钟上贴图
	hdc = CreateCompatibleDC(hDC);
	switch((sundate.nYear-4)%12)
	{
	case 0:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/0.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 1:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/1.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 2:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/2.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 3:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/3.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 4:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/4.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 5:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/5.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 6:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/6.bmp",IMAGE_BITMAP,17,17,LR_LOADFROMFILE);
		break;
	case 7:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/7.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 8:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../image/shengxiao/8.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 9:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/9.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 10:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/10.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	case 11:
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/shengxiao/11.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		break;
	}
	SelectObject(hdc,hBkmap);
	BitBlt(hDC,55,100,17,17,hdc,0,0,SRCCOPY);
	}

