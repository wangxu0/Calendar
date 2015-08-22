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
API�����ο���
GetLocalTime: ��õ�ǰʱ��(�������ꡢ�¡��ա���)
MoveToEx:     �ƶ����ߵ����
LineTo:       ���Ƶ�ĳһ��
SetTextColor: ����Ҫ���Ƶ�������ɫ
TextOut:      ��������
SetBkMode:   ���ñ�����ʾģʽ
wsprintf:    ����ת��Ϊ�ַ��������ַ�����Unicode��
GetClientRect:  ��ȡ���ڴ�С
GetDC:   ��ȡ�ڴ��豸
SetTimer:  ���ö�ʱ��
FillRect:  ������
CreateSolidBrush:  ����ˢ��
RGB:   ����RGB��ɫ
CreateFont:����һ���������Ե�����
�ṹ�����Ͳο���
SYSTEMTIME:   ����ʱ�����ͣ����������ա�ʱ���롢��
*********************************************************
*/

HINSTANCE g_hInst;//�������ϵͳ���ݽ�����"Ӧ�ó���ʵ�����"

//Windows������ں���
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
//�����񣬴洢1901��-2099���ũ����Ϣ��ÿ��Ĵ�С�¡�������Ϣ���������ڵĹ�������
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
	"����","�ҳ�","����","��î","�쳽","����","����","��δ","����","����",
	"����","�Һ�","����","����","����","��î","����","����","����","��δ",
	"����","����","����","����","����","�ѳ�","����","��î","�ɳ�","����",
	"����","��δ","����","����","����","�Ѻ�","����","����","����","��î",
	"�׳�","����","����","��δ","����","����","����","����","����","���",
	"����","��î","����","����","����","��δ","����","����","����","�ﺥ "
};
char *ChDayTable[] = 
{
	"*","��һ","����","����","����","����",
	"����","����","����","����","��ʮ",
	"ʮһ","ʮ��","ʮ��","ʮ��","ʮ��",
	"ʮ��","ʮ��","ʮ��","ʮ��","��ʮ",
	"إһ","إ��","إ��","إ��","إ��",
	"إ��","إ��","إ��","إ��","��ʮ"
};

char *ChMonthTable[] = {"*","��","��","��","��","��","��","��","��","��","ʮ","ʮһ","��"};

//�����񣬴洢ÿ��1����Ԫ�������������������꣩
unsigned int MonthAdd[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

//�����û�Ҫ��ʾ���ꡢ�¡���
unsigned int nCurrentYear=0;
unsigned int nCurrentMonth=0;
unsigned int nCurrentDay=0;


//�ж�ĳ���Ƿ�Ϊ��������
int IsLeapYear(WORD iYear)   
{
	if(iYear%4==0&&iYear%100!=0)
		return 1;
	if(iYear%400==0)
		return 1;//�Ѿ��Ĺ���
	return 0;
}

//��������תũ�����ڵĺ���
void SunToLunar(const stDateTime* pSunDay,stDateTime* pLunarDay)
{
	int i=0;
	int SunDiff=0;
	int SpringDiff=0;

	int nSpringYear=pSunDay->nYear;
	int nSpringMonth=1;
	int nSpringDay=0;

	//���㵱ǰ�������뵱��Ԫ��������
	SunDiff = MonthAdd[pSunDay->nMonth-1] + pSunDay->nDay - 1;
	if(IsLeapYear(pSunDay->nYear) && (pSunDay->nMonth > 2) )//���������Ҵ���2�£������ټ�1
		SunDiff++;

	//���㴺�����ڹ�������Ԫ����������ͨ��������㵱��Ĵ������ڵĹ������ڣ�
	if(((LunarCalendarTable[pSunDay->nYear - 1901] & 0x60)>>5)==1)//�������ڹ���1��
		SpringDiff= (LunarCalendarTable[pSunDay->nYear-1901]&0x1F)-1;
	else//�������ڹ���2��
		SpringDiff= (LunarCalendarTable[pSunDay->nYear-1901]&0x1F)-1+31;

	//SunDiff��SpringDiff�Ĳ���ǵ�ǰ�������봺�ڵ�����

	if(SunDiff>=SpringDiff)//�������ڴ��ں�
	{
		int x = SunDiff - SpringDiff;
		int bLunar = (LunarCalendarTable[nSpringYear - 1901] & 0xF00000)>>20;//�����·�

		i=0;
		nSpringMonth=1;

		for(;;)
		{
			int a = (LunarCalendarTable[pSunDay->nYear-1901] & (0x80000>>i));//�����С��
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
		//���������
		if(nSpringMonth>bLunar && bLunar>0)
			nSpringMonth--;
	}
	else//�������ڴ���ǰ
	{

		int x = SpringDiff-SunDiff;
		int nShift=0;

		int bLunar = (LunarCalendarTable[nSpringYear - 1901] & 0xF00000)>>20;//�����·�

		i=0;
		nSpringYear--;//��Ҫ��һ���ũ����Ϣ
		nSpringMonth=12;//�����һ���¿�ʼ����

		if (bLunar == 0)//���û������
			nShift = 0x100;
		else
			nShift = 0x80;

		for(;;)
		{
			int a = (LunarCalendarTable[nSpringYear-1901] & (nShift<<i));//�����С��
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
//���ݹ������ڻ������
int GetDayOfWeek(const stDateTime* pSunDay)
{
	//����̩�չ�ʽ
	//nWeek = [nYear1��4] - 2*nYear1 + nYear2 + [nYear2��4] + [26(nMonth+1)��10] + nDay - 1
	int nWeek=0;
	int nYear1=pSunDay->nYear/100;//���ǰ��λ����
	int nYear2=pSunDay->nYear%100;//��ĺ���λ����
	int nDay=pSunDay->nDay;  //������

	int nMonth=pSunDay->nMonth;//����������������·������1�»�2�£�
	//��Ӧ��Ϊ��һ���13�»�14�£�
	//Ҳ���ǹ�ʽ��nMonth��ȡֵ��Χ�Ǵ�3��14�����Ǵ�1��12

	if(nMonth==1 || nMonth==2)
	{
		nMonth=12+nMonth;
		nYear1=(pSunDay->nYear-1)/100;//���ǰ��λ����
		nYear2=(pSunDay->nYear-1)%100;//��ĺ���λ����
	}

	nWeek = nYear1/4 - 2*nYear1 + nYear2 + nYear2/4 + (26*(nMonth+1))/10 + nDay - 1;
	if(nWeek < 0)
		nWeek = ((nWeek)%7+7);//     
	else
		nWeek = (nWeek)%7;
	return nWeek;
}



//���ݹ�����õ�ũ����֧��
char* GetGanZhi(int nYear)
{
	return TianGanDizhi[nYear%60-3-1];
}

//�������ƺ���
int count=1;
void DrawCalendar(HDC hDC,int nWidth,int nHeight)
{
    SYSTEMTIME DateTime;//����ʱ��ṹ�����
    stDateTime SunDay,LunarDay,sundate_today;//�������������������������
	RECT		rc;
	HBRUSH hBrush;//ˢ��
	HFONT   hFont;
	HBITMAP hbmp=NULL;
	HBITMAP m_hB2;//��ͼ
	HBITMAP hBkmap;
	HDC hdc;//�豸���
	HPEN hpen;  //��
	const int xRight  = nWidth - 50;
	const int yBottom = nHeight- 10;
	const char *week[]={"������","����һ","���ڶ�","������","������","������","������"};
	const char *weekEng[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    char str[3],tt[20],ttt[100],tttt[100],str1[3];
	int days=0,Height=0,Width=0,k=0, w=0,i;

   

//������ͼƬ��
	hdc = CreateCompatibleDC(hDC);//�ú�������һ����ָ���豸���ݵ��ڴ��豸�����Ļ���(DC)
		hBkmap = (HBITMAP)LoadImage(g_hInst,
			"../Image/skin/a5.bmp",IMAGE_BITMAP,545,430,LR_LOADFROMFILE);
    SelectObject(hdc,hBkmap);
	BitBlt(hDC,0,0,545,430,hdc,0,0,SRCCOPY);
	   

//������������ľ��Σ���ӱ�����ɫ������  ������ͼ��
	//rc.left=0;
	//rc.top=0;
	//rc.bottom=450;
	//rc.right=550;
	//hBrush=CreateSolidBrush(RGB(0,255,0));
	//FillRect(hDC,&rc,hBrush); //������

//��ȡ��ǰʱ��
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


//�������
	
		hFont=CreateFont(20,20,0,0,FW_REGULAR,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS, "����������ʽ");    //���������ʽ ����һ���������Ե��߼�����
        SelectObject(hDC,hFont);
	    SetTextColor(hDC,RGB(255,70,70));
	    wsprintf(tttt,"�����˳");
		TextOut(hDC,30,5,tttt,strlen(tttt));

		hFont=CreateFont(20,20,0,0,FW_REGULAR,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS, "����������ʽ");    //���������ʽ ����һ���������Ե��߼�����
        SelectObject(hDC,hFont);
	    SetTextColor(hDC,RGB(255,70,70));
        wsprintf(tttt,"����ƽ��");
        TextOut(hDC,28,25,tttt,strlen(tttt)); 
	
	   
//�����Ͻ��������
	hFont=CreateFont(14,12,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,"����������ʽ");
    SelectObject(hDC,hFont);
	SetTextColor(hDC,RGB(255,0,0));
    wsprintf(ttt,"%.4d��  %.2d��",SunDay.nYear,SunDay.nMonth);
	TextOut(hDC,270,25,ttt,strlen(ttt));
	 
    

////����������ĸ�ʽ���
//	rc.left=21;//���������ڵľ���
//	rc.top=55;
//	rc.bottom=105;
//	rc.right=489;
//	hBrush=CreateSolidBrush(RGB(0,255,0));
//	FillRect(hDC,&rc,hBrush); //������


//���Ʊ��
	for(i = 55; i < 110; i += 25)//�����ߣ����·�Χ
	{
		MoveToEx (hDC, 20, i, NULL);//x��������
		LineTo (hDC, 489, i);//x������Ҷ�
	}
    for(i = 105; i < 450; i += 50)//�����ߣ����·�Χ
	{
		MoveToEx (hDC, 20, i, NULL);//x��������
		LineTo (hDC, 489, i);//x������Ҷ�
	}
    for(i = 20; i <= 489; i += 67)
	{
        MoveToEx (hDC, i, 55, NULL);//y������϶�
		LineTo (hDC, i, 405);//y������¶�
	}


//�ڱ���·����ũ������Ϣ
	hFont=CreateFont(14,12,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,"����������ʽ");
    SelectObject(hDC,hFont);
	SetTextColor(hDC,RGB(0,0,0));
	switch(SunDay.nYear%12)
	{
	case 0:
	       TextOut(hDC,122,410,"����",4);break;
	case 1:
		   TextOut(hDC,122,410,"����",4);break;
	case 2:
		   TextOut(hDC,122,410,"����",4);break;
	case 3:
		   TextOut(hDC,122,410,"����",4);break;
	case 4:
		   TextOut(hDC,122,410,"����",4);break;
	case 5:
		   TextOut(hDC,122,410,"ţ��",4);break;
    case 6:
		   TextOut(hDC,122,410,"����",4);break;
    case 7:
		   TextOut(hDC,122,410,"����",4);break;
    case 8:
	       TextOut(hDC,122,410,"����",4);break;     
	case 9:
		   TextOut(hDC,122,410,"����",4);break;
	case 10:
		   TextOut(hDC,122,410,"����",4);break;
	case 11:
		   TextOut(hDC,122,410,"����",4);break;//���ַ��ĳ�ASCII��Ϊʲô�����أ���������
	}
	TextOut(hDC,30,410,"ũ��",4);
	TextOut(hDC,76,410,GetGanZhi(SunDay.nYear),4);//���ò��ˣ���Ҫ�޸ġ�//������ϣ�����//��Ҫ��ɫ~~
	//TextOut(hDC,106,360,)
   
//��Ф��ͼ
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

	


//�������Ϸ�������
    hFont=CreateFont(11,8,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,"����������ɫ");
    SelectObject(hDC,hFont);
	SetTextColor(hDC,RGB(0,255,0));//��ƺ�����ɫ���������һ��������
	for(i=1;i<6;i++)
	{
		TextOut(hDC,30+67*i,60,week[i],6);
		TextOut(hDC,30+67*i,85,weekEng[i],3);
	}
    SetTextColor(hDC,RGB(255,0,0));//��Ϊ��ɫ
	TextOut(hDC,30,60,week[0],6); 
	TextOut(hDC,30,85,weekEng[0],3);
    TextOut(hDC,440,60,week[6],6);
	TextOut(hDC,440,85,weekEng[6],3);//��ơ������ա��͡���������λ�ü���ɫ


//�������
    	SunToLunar(&SunDay,&LunarDay);//��õ�ǰ������ת��������
	if(SunDay.nMonth ==1||SunDay.nMonth ==3||SunDay.nMonth ==5||SunDay.nMonth ==7||SunDay.nMonth ==8||SunDay.nMonth ==10||SunDay.nMonth ==12)
		days=31;
	else if(SunDay.nMonth ==4||SunDay.nMonth ==6||SunDay.nMonth ==9||SunDay.nMonth ==11)
		days=30;
	else if(SunDay.nMonth ==2&&IsLeapYear(SunDay.nYear))
	    days=29;
	else
		days=28;
	    w=GetDayOfWeek(&sundate_today);//���ݹ������ڻ������
		for(i=1;i<=days;i++)
	{   
		SunDay.nDay=i;
	    SunToLunar(&SunDay,&LunarDay);//��ù�������תũ������
		hFont=CreateFont(15,10,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_DONTCARE,"���������ʽ");//��������ʹ�С
        SelectObject(hDC,hFont);//ѡ�����õ�ˢ��
		Width=(w+i-1)%7;
	    Height=((w+i-1)-(w+i-1)%7)/7;
//��䵱�����ڵı�����ɫ�����ʾ�ͷ�ֹ���ǵ����á�
		if(i==DateTime.wDay && nCurrentYear==DateTime.wYear && nCurrentMonth==DateTime.wMonth)
		{
			rc.left=20+(Width)*67;
	        rc.top=105+(Height)*50;
	        rc.right=20+(Width+1)*67;
	        rc.bottom=105+(Height+1)*50;
			hBrush=CreateSolidBrush(RGB(255,10,0));
	        FillRect(hDC,&rc,hBrush);
	    }


		

//�ֱ������������������
		
			wsprintf(str,"%d",i);
		    SetTextColor(hDC,RGB(0,0,0));
		    TextOut(hDC,30+(Width)*70,110+Height*50,str,strlen(str));  //����
			TextOut(hDC,20+(Width)*70,135+Height*50,ChDayTable[LunarDay.nDay],4);//����



//��ʾ������
			hFont=CreateFont(14,9,0,0,0,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,DEFAULT_PITCH | FF_MODERN, "���������ʽ");    //���������ʽ
	      SelectObject(hDC,hFont);
		  SetTextColor(hDC,RGB(255,50,247));//�趨������յ���ɫ
         if (LunarDay.nMonth==1&&LunarDay.nDay==1)
		 {
			 wsprintf(tt,"����");//���ַ��ĳɣ��ӣãɣ������ԡ�������
			 TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 } 
		 if (LunarDay.nMonth==1&&LunarDay.nDay==15)
		 {   
			 wsprintf(tt,"Ԫ����");
		     TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		   if (SunDay.nMonth==4&&SunDay.nDay==5)
		 {
			  wsprintf(tt,"������");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if (LunarDay.nMonth==5&&LunarDay.nDay==5)
		 {
			 wsprintf(tt,"�����"); 
			 TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
      		  if (LunarDay.nMonth==8&&LunarDay.nDay==15)
		 {
			  wsprintf(tt,"�����");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==1&&SunDay.nDay==1)
		 {
			  wsprintf(tt,"Ԫ��");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==2&&SunDay.nDay==14)
		 {
			  wsprintf(tt,"���˽�");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==3&&SunDay.nDay==8)
		 {
			  wsprintf(tt,"��Ů��");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
	     if (SunDay.nMonth==4&&SunDay.nDay==1)
		 {
			  wsprintf(tt,"���˽�");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
         if(SunDay.nMonth==5&&SunDay.nDay==1)
		 {
		      wsprintf(tt,"�Ͷ���");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==6&&SunDay.nDay==1)
		 {
			  wsprintf(tt,"��ͯ��");
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==10&&SunDay.nDay==1)
		 {
			  wsprintf(tt,"�����");             
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		 if(SunDay.nMonth==11&&SunDay.nDay==11)
		 {
			  wsprintf(tt,"�����");             
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
		  if(SunDay.nMonth==12&&SunDay.nDay==25)
		 {
			  wsprintf(tt,"ʥ����");             
		      TextOut(hDC,13+Width*70,123+Height*50,tt,strlen(tt));
		 }
	}
	
}


//�ú����������ϵͳ�Զ�����DrawCalendar����
//      ��ˣ��������Ϊ���ú�����DrawCalendar������������
void CalendarButtonEvent(UINT nID)     
{
	switch (nID)
	{
	case IDC_BTN_YEARUP://�����"���"��ť	
		nCurrentYear++;
		break;
	case IDC_BTN_YEARDOWN:
		nCurrentYear--;
		break;
	case IDC_BTN_MONTHUP:
		nCurrentMonth++;
		if(nCurrentMonth>12)//�Լ������ġ�
		{
			nCurrentMonth-=12;
			nCurrentYear++;
		}
		break;
	case IDC_BTN_MONTHDOWN:
		nCurrentMonth--;
		if(nCurrentMonth<=0)//�Լ������ġ�
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


//�ú���ÿ0.5�뱻����һ��,ʱ�ӽ����ϵ����ֻ���
void DrawClockText(HDC hDC)
{ // HFont hf;
  // hf=CreateFont();
   SYSTEMTIME datetime;int i;
   /*�Լ�д��*/
   char tttt[100];                                      //���ʱ����м������
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
   sundate.nDay=datetime.wDay;//��ȡ��ǰϵͳ����ʱ�䡣
   sundate.nMonth =datetime.wMonth;
   sundate.nYear=datetime.wYear;
   wsprintf(str,"%d",sundate.nYear);

//�Ƕ����������
   newFont=CreateFont(10,9,480,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,  //�߶ȿ����λ��x��Ƕ�
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"����");
    SelectObject(hDC,newFont);//ѡ���õ�ˢ��
    SetTextColor(hDC,RGB(255,0,0));
	TextOut(hDC,15,40,str,4); //�ַ�������x,y��׼�㣬���ַ���
    wsprintf(str,"%.2d",sundate.nMonth);

    newFont=CreateFont(10,9,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"����");//��CreateFont���������Ƕȡ����ִ�С�����͡�
    SelectObject(hDC,newFont);//ѡ���õ�ˢ��
    TextOut(hDC,61,12,str,2);
	 wsprintf(str,"%.2d",sundate.nDay);

    newFont=CreateFont(10,9,-480,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"����");
    SelectObject(hDC,newFont);//ѡ���õ�ˢ��
    TextOut(hDC,95,17,str,2);

//�������ڵ���ʾ��
   SunToLunar(&sundate,&lunardate);
   i=lunardate.nMonth;
   //TextOut(hDC,20,20,ChMonthTable[i],2);//ũ��������ʲô�ط���ʾ������������
   //TextOut(hDC,30,20,"��",2);
   
//ϵͳʱ�����ʾ��
   hpen=(HPEN)CreateFont(19,9,0,0,9,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"����");
	SelectObject(hDC,hpen);
	SetTextColor(hDC,RGB(255, 255, 0));
    rc.left=28;               //���ʱ��ı����Է�ֹ���ǣ�ģ����
	rc.top=48;   
	rc.bottom=65;
	rc.right=100;
	hBrush=CreateSolidBrush(RGB(12,45,78));  //��ʾʱ��ľ���
	FillRect(hDC,&rc,hBrush); //������

    

	wsprintf(tttt,"%.2d:%.2d:%.2d",datetime.wHour,datetime.wMinute,datetime.wSecond); //�������ʱ��ĸ�ʽ��ϵͳʱ�����tttt�С�
    //system("CLS");//����ˢ��������˸������⡣
	//clrscr();
	TextOut(hDC,28,48,tttt,strlen(tttt)); 


//���ʱ��ı����Է�ֹ���ǣ�ģ����
    rc.left=34;   //���Ͻ�x            
	rc.top=70;     //���Ͻ�y
	rc.bottom=85;  //���½�y
	rc.right=93;   //���½�x
	hBrush=CreateSolidBrush(RGB(12,45,78));  
	FillRect(hDC,&rc,hBrush); //������

//�������
	newFont=CreateFont(16,8,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"���������ʽ");
    SelectObject(hDC,newFont);
	SetTextColor(hDC,RGB(0,0,255));

	switch(datetime.wDayOfWeek)
	{
	case 0:
		 TextOut(hDC,46,70,"����",4);break;//����λ�ò�������ȷ���������Ϣ��
	case 1:
		 TextOut(hDC,46,70,"��һ",4);break;
	case 2:
		 TextOut(hDC,46,70,"�ܶ�",4);break;
	case 3:
		 TextOut(hDC,46,70,"����",4);break;
	case 4:
		 TextOut(hDC,46,70,"����",4);break;
	case 5:
		 TextOut(hDC,46,70,"����",4);break;
	case 6:
		 TextOut(hDC,46,70,"����",4);break;
	}

//�����Ф��
	 newFont=CreateFont(15,7,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
         CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"����");
    SelectObject(hDC,newFont);
	SetTextColor(hDC,RGB(0,255,0));
	 switch(sundate.nYear%12)
	{
	case 0:
	       TextOut(hDC,47,30,"����",4);break;
	case 1:
		   TextOut(hDC,47,30,"����",4);break;
	case 2:
		   TextOut(hDC,47,30,"����",4);break;
	case 3:
		   TextOut(hDC,47,30,"����",4);break;
	case 4:
		   TextOut(hDC,47,30,"����",4);break;
	case 5:
		   TextOut(hDC,47,30,"ţ��",4);break;
    case 6:
		   TextOut(hDC,47,30,"����",4);break;
    case 7:
		   TextOut(hDC,47,30,"����",4);break;
    case 8:
	       TextOut(hDC,47,30,"����",4);break;     
	case 9:
		   TextOut(hDC,47,30,"����",4);break;
	case 10:
		   TextOut(hDC,47,30,"����",4);break;
	case 11:
		   TextOut(hDC,47,30,"����",4);break;
	}	
	TextOut(hDC,47,85,GetGanZhi(datetime.wYear),4);


//ʱ������ͼ
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

