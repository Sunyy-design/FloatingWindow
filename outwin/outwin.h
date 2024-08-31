// outwin.h : outwin DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include <comdef.h>
#include "gdiplus.h"

using namespace Gdiplus;

#define TIMER_SEC 1			// 秒针计时器        
#define TIMER_MIN 2			// 分针计时器
#define TIMER_FRA 3			// 帧刷新计时器

#define OUTWND_STATE_MOUSEOUT  0		//状态机鼠标未进入
#define OUTWND_STATE_MOUSEIN  1			//状态机鼠标进入
#define OUTWND_STATE_FAN  2				//状态机扇形区打开



#ifdef __cplusplus
        
#define EXPORT extern "C" __declspec (dllexport)
        
#else
        
#define EXPORT __declspec (dllexport)
        
#endif

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);
// 外部窗口消息函数
LRESULT CALLBACK OutWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

// 定义按钮相应参数函数类型
typedef void (*BTNCALL)(LPVOID);

// 三角按钮回调函数
void SBtn_Call(void *p);
// wnd按钮函数
void WBtn_Call(void *p);

/**** 
* 按钮类
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
class MButton
{
public:
	MButton();
	MButton(PTCHAR inpath, PTCHAR outpath,int x,int y,bool show,BTNCALL lpBtnCall,void* pAgv);
private:
	Image *m_pmousein;			// 鼠标在它上面时显示的图像
	Image *m_pmouseout;			// 鼠标不在
	BTNCALL m_lpBtnCall;		// 按钮调用函数
	void* m_lpBtnCallAgv;		// 按钮调用函数参数
	int m_nwidth;				// 图像宽
	int m_nheight;				// 图像高
	int m_nx;					// 按钮posx
	int m_ny;					// 按钮posy

	
	RECT rct;					// 按钮判定区域

private:
	bool IsMouseOn();	// 判断函数鼠标是否在按钮上
public:
	void OnPaint(Graphics *graph);	// 每帧重绘函数
	void OnLButtonUp();			// 当鼠标左键弹起

	bool m_bishow;				// 是否显示
};

// CoutwinApp
// 有关此类实现的信息，请参阅 outwin.cpp
//

class CoutwinApp : public CWinApp
{
public:
	CoutwinApp();

// 重写
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()

// 私有对象
private:
	HINSTANCE m_hInstance;			// 记录本dll的程序句柄
	
	HWND m_OutWindow;				// 记录外部窗口wnd
	HWND m_hFartherWnd;				// 记录父类窗口wnd

	// GDI+初始化对象
	GdiplusStartupInput   m_gdiplusStartupInput; 
	ULONG_PTR   m_gdiplusToken;
public:
	BLENDFUNCTION m_Blend;				// 变形窗口参数
	Image *m_pImageBack;				// 背景图像指针，Image是一个图像类。
	Image *m_pBGMouseIn;
	Image *m_pBGMouseOut;
	Image *m_pAlaram;
	Image *m_pBGFan;						// 扇形面板背景
	int m_BakWidth;						// 图像宽高
	int m_BakHeight;
	HANDLE m_hThreadRec;			// 帧刷新线程

	int m_noutwndstate;				// 外部窗口状态机
	int mousex,mousey;					// 鼠标在外部窗口上的坐标

	LPCWSTR m_sbig;					// 大圆内显示的文字
	LPCWSTR m_sup;					// 上箭头后的文字
	LPCWSTR m_sdown;					// 下箭头后的文字

public:
	void SetFartherWnd( HWND fwnd ){ m_hFartherWnd = fwnd; }	
	HINSTANCE GetInstance(){return m_hInstance;};
	ULONG_PTR GetGDIToken(){ return m_gdiplusToken; }
	HWND GetOutWindow(){return m_OutWindow; }
	void SetOutWindow( HWND wnd );
	void OnMePaint( HDC hdc,Graphics *gra , Point *p);
	bool m_bIsCreate;							// 记录窗口是否创建

	void OnMELButtonUp();

	MButton *m_pSBtn;							// 三角形btn
	MButton *m_pWBtn;							// win形btn
};

CoutwinApp* GetApp();

/**** 导出函数
* 函数功能：外部调用命令初始化气泡窗口
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void OutWindowInit( HWND wnd );

/**** 导出函数
* 函数功能：显示气泡窗口
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void ShowOutWindow();

/**** 导出函数
* 函数功能：隐藏气泡窗口
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void HideOutWindow();

/**** 导出函数
* 函数功能：关闭气泡窗口，释放资源
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void CloseOutWindow();

/**** 导出函数
* 函数功能：设置位置
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void SetOutWndPos(float x, float y);

/**** 导出函数
* 函数功能：改变标题
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void SetOutWindowTile(LPCSTR stile);

/**** 导出函数
* 函数功能：报警
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void OutWindowAlarm();

/**** 导出函数
* 函数功能：取消报警
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void CancelOutWindowAlarm();
