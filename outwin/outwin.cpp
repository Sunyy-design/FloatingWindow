// outwin.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "outwin.h"

using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")


#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CoutwinApp

BEGIN_MESSAGE_MAP(CoutwinApp, CWinApp)
END_MESSAGE_MAP()

// CoutwinApp 构造

CoutwinApp::CoutwinApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	m_noutwndstate = OUTWND_STATE_MOUSEOUT;
	m_pSBtn = NULL;
	m_sbig = L"90%\n";
	m_sup = L"85k/s\n";
	m_sdown = L"3B/s\n";
}


// 唯一的一个 CoutwinApp 对象

CoutwinApp theApp;

CoutwinApp* GetApp()
{
	return &theApp;
}

const GUID CDECL _tlid = { 0x5B1C4987, 0x8110, 0x4525, { 0x92, 0x5A, 0x14, 0x5E, 0xF5, 0x24, 0x2, 0xB4 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

MButton::MButton()
{
}

MButton::MButton(PTCHAR inpath, PTCHAR outpath,int x,int y,bool show,BTNCALL lpBtnCall,void* pAgv)
{
	m_pmousein = Gdiplus::Image::FromFile(inpath);
	m_pmouseout = Gdiplus::Image::FromFile(outpath);
	m_nwidth = m_pmousein->GetWidth();
	m_nheight = m_pmousein->GetHeight();
	m_nx = x;
	m_ny = y;

	// 按钮所占区域
	rct.left = m_nx;
	rct.right = m_nx+m_nwidth;
	rct.top = m_ny;
	rct.bottom = m_ny+m_nheight;

	m_bishow = show;

	m_lpBtnCall = lpBtnCall;
	m_lpBtnCallAgv = pAgv;
}

// 判断鼠标是否在按钮上
bool MButton::IsMouseOn()
{		
	if( ( GetApp()->mousex >= rct.left ) && (GetApp()->mousex <= rct.right )  )
	{
		if( (GetApp()->mousey >= rct.top) && ( GetApp()->mousey <= rct.bottom ) )
		{
			return true;
		}
	}
	return false;
}

void MButton::OnPaint(Graphics *graph)
{
	// 按钮是否显示
	if(m_bishow)
	{
		if(IsMouseOn())
			graph->DrawImage(m_pmousein,m_nx,m_ny);
		else
			graph->DrawImage(m_pmouseout,m_nx,m_ny);
	}
}

void MButton::OnLButtonUp()
{
	// 按钮是否显示
	if(m_bishow)
	{
		//鼠标是否在它上面
		if(IsMouseOn())
		{
			// 调用按钮点击的函数
			m_lpBtnCall(m_lpBtnCallAgv);
		}
	}
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hWnd  = CreateWindowEx( WS_EX_WINDOWEDGE
		,L"OutWindow"
		,TEXT("气泡窗口")
		,WS_OVERLAPPEDWINDOW | WS_SYSMENU| WS_THICKFRAME
		,0,0,0,0
		,NULL,NULL,hInstance,NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	theApp.SetOutWindow(hWnd);
 
	// 禁用最小化 最大化按钮
	::EnableMenuItem( GetSystemMenu(hWnd,FALSE) , SC_MINIMIZE , MF_BYCOMMAND|MF_GRAYED );
	::EnableMenuItem( GetSystemMenu(hWnd,FALSE) , SC_MAXIMIZE , MF_BYCOMMAND|MF_GRAYED );

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

DWORD WINAPI ThreadFRA(LPVOID lpParameter)
{
	HINSTANCE hInstance = (HINSTANCE)lpParameter;

	MSG msg;
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, SW_SHOW))
	{
		return FALSE;
	}

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}


// CoutwinApp 初始化

BOOL CoutwinApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 将所有 OLE 服务器(工厂)注册为运行。这将使
	//  OLE 库得以从其他应用程序创建对象。
	COleObjectFactory::RegisterAll();

	// GDI+函数初始化
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	// 获得本程序的句柄
	m_hInstance = AfxGetInstanceHandle();

	m_bIsCreate = false;

	return TRUE;
}

// 三角按钮回调函数
void SBtn_Call(void *p)
{
	theApp.m_noutwndstate = OUTWND_STATE_FAN;
	return;
}

// 三角按钮回调函数
void WBtn_Call(void *p)
{
	MessageBox(theApp.GetOutWindow(),L"哇哈哈哈",NULL,MB_OK);
	return;
}

// 设定wnd窗体 暂时把初始化工作也放在这里
void CoutwinApp::SetOutWindow( HWND wnd )
{
	m_OutWindow = wnd;
	m_bIsCreate = true;
	// 初始化三角按钮
	m_pSBtn = new MButton(L"sbtnin.png",L"sbtnout.png",115,94,true,SBtn_Call,NULL);
	m_pWBtn = new MButton(L"btnwndin.png",L"btnwndout.png",86,69,true,WBtn_Call,NULL);
}

void CoutwinApp::OnMePaint( HDC hdc,Graphics *graph , Point *points)
{
	switch(m_noutwndstate)
	{
	case OUTWND_STATE_MOUSEOUT:
		theApp.m_pImageBack = theApp.m_pBGMouseOut;
		graph->DrawImage(m_pImageBack, points,3);
		m_pSBtn->m_bishow = false;
		m_pWBtn->m_bishow = false;
		break;
	case OUTWND_STATE_MOUSEIN:
		theApp.m_pImageBack = theApp.m_pBGMouseIn;
		graph->DrawImage(m_pImageBack, points,3);
		m_pSBtn->m_bishow = true;
		break;
	case OUTWND_STATE_FAN:
		graph->DrawImage(m_pBGFan, points,3);
		theApp.m_pImageBack = theApp.m_pBGMouseIn;
		graph->DrawImage(m_pImageBack, points,3);
		m_pSBtn->m_bishow = true;
		m_pWBtn->m_bishow = true;
		break;
	}
	// 按钮绘制
	m_pSBtn->OnPaint(graph);
	m_pWBtn->OnPaint(graph);
	// 文字绘制,考虑使用图片数字显示方式
}

void CoutwinApp::OnMELButtonUp()
{
	if( NULL != m_pSBtn )
	{
		m_pSBtn->OnLButtonUp();
	}
	if( NULL != m_pWBtn )
	{
		m_pWBtn->OnLButtonUp();
	}
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW ;
	wcex.lpfnWndProc	= OutWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(hInstance,L"mouse.cur");
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"OutWindow";
	wcex.hIconSm		= NULL;
	return RegisterClassEx(&wcex);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DllGetClassObject - 返回类工厂

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}


// DllCanUnloadNow - 允许 COM 卸载 DLL

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return S_OK;
}


// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* 外部聊天窗口消息函数
*/
LRESULT CALLBACK OutWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int x,y;
	HDC hdcMemory,hdcScreen;
	HBITMAP hBitMap;
	RECT rct;
	POINT ptWinPos;
	Graphics *graph;
	Point points[3];
	POINT ptSrc={0,0};
	SIZE sizeWindow;
	DWORD dwExStyle;
	POINT scpoint;
	LONG lStyle;

	BOOL _bMouseTrack=TRUE ;

	static POINT pt, pe;
	static RECT rt, re;

	// TODO: 在此添加任意绘图代码...	

	switch (message) {
	case WM_CREATE:
		// 在任务栏隐藏
		SetWindowLong(hWnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW|WS_EX_STATICEDGE);

		// 固定窗口大小
		lStyle= GetWindowLong(hWnd,GWL_STYLE);
		lStyle&=~WS_THICKFRAME;
		lStyle&=~WS_MAXIMIZEBOX;
		lStyle&=~WS_MINIMIZEBOX;
		lStyle&=~WS_CAPTION;
		((LPCREATESTRUCT)lParam)->cx=50;
		((LPCREATESTRUCT)lParam)->cy=100;
		SetWindowLong(hWnd,GWL_STYLE,lStyle);

		// 设定窗口在最上层
		SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

		SetTimer (hWnd, TIMER_FRA, 30, NULL) ;
		SetTimer (hWnd, TIMER_SEC, 1000, NULL) ;        
		SetTimer (hWnd, TIMER_MIN, 60000, NULL) ;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_SEC:
			//每秒一次的处理
			break ;
		case TIMER_MIN:
			//每分钟一次的处理
			break ;
		case TIMER_FRA:
			//每帧一次的处理
			InvalidateRect(hWnd,NULL,true);
			break ;
		}
		break;
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);							// 设置鼠标捕获(防止光标跑出窗口失去鼠标热点)
		GetCursorPos(&pt);							// 获取鼠标光标指针当前位置
		GetWindowRect(hWnd,&rt);					// 获取窗口位置与大小
		re.right=rt.right-rt.left;					// 保存窗口宽度
		re.bottom=rt.bottom-rt.top;					// 保存窗口高度
		break;
	case WM_MOUSEMOVE:
		GetCursorPos(&pe);											// 获取光标指针的新位置
		if(wParam==MK_LBUTTON)										// 当鼠标右键按下
		{
			re.left=rt.left+(pe.x - pt.x);							// 窗口新的水平位置
			re.top =rt.top+(pe.y - pt.y);							// 窗口新的垂直位置
			MoveWindow(hWnd,re.left,re.top,re.right,re.bottom,true);// 移动窗口
		}
		// 启用系统的 鼠标停留 和 鼠标离开消息
		if (_bMouseTrack)     // 若允许 追踪，则。 
		{
			TRACKMOUSEEVENT csTME;
			csTME.cbSize = sizeof (csTME);
			csTME.dwFlags = TME_LEAVE|TME_HOVER;
			csTME.hwndTrack = hWnd ;				// 指定要 追踪 的窗口 
			csTME.dwHoverTime = 10;					// 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER
			::_TrackMouseEvent (&csTME);			// 开启 Windows 的 WM_MOUSELEAVE ， WM_MOUSEHOVER 事件支持

			_bMouseTrack=FALSE ;					// 若已经 追踪 ，则停止 追踪 
		}
		break;
	case WM_MOUSEHOVER:
		theApp.mousex = LOWORD(lParam);
		theApp.mousey = HIWORD(lParam);
		if( OUTWND_STATE_MOUSEOUT == theApp.m_noutwndstate )
		{
			// 鼠标停留消息
			theApp.m_noutwndstate = OUTWND_STATE_MOUSEIN;
		}
		break;
	case WM_MOUSELEAVE:
		// 鼠标离开消息，若不在WM_MOUSEMOVE中开启则无效
		theApp.m_noutwndstate = OUTWND_STATE_MOUSEOUT;
		theApp.mousex = 0;
		theApp.mousey = 0;
		break;
	case WM_LBUTTONUP:
		theApp.OnMELButtonUp();
		ReleaseCapture();                           // 释放鼠标捕获，恢复正常状态
		break;
	case WM_RBUTTONUP:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_MOVE:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_SYSCOMMAND:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_COMMAND:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//自定义绘制代码
		hdcMemory = CreateCompatibleDC(hdc);
		hBitMap = CreateCompatibleBitmap(hdc, theApp.m_BakWidth, theApp.m_BakHeight);
		SelectObject(hdcMemory, hBitMap);
		hdcScreen = ::GetDC(hWnd);
		GetWindowRect(hWnd,&rct);

		ptWinPos.x = rct.left;
		ptWinPos.y = rct.top;

		graph = new Graphics(hdcMemory);

		points[0] =	Point(0, 0);
		points[1] =	Point(theApp.m_BakWidth, 0);
		points[2] =	Point(0, theApp.m_BakHeight);

		//绘哪些图 就交给theapp了
		theApp.OnMePaint(hdcMemory,graph,points);

		//打印
		sizeWindow.cx = theApp.m_BakWidth;
		sizeWindow.cy = theApp.m_BakHeight;

		dwExStyle=GetWindowLong(hWnd,GWL_EXSTYLE);

		if((dwExStyle&0x80000)!=0x80000)
			SetWindowLong(hWnd,GWL_EXSTYLE,dwExStyle^0x80000);

		UpdateLayeredWindow(hWnd, hdcScreen, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &theApp.m_Blend, 2);

		graph->ReleaseHDC(hdcMemory);
		delete graph;
		::ReleaseDC(hWnd,hdcScreen);
		hdcScreen=NULL;
		DeleteObject(hBitMap);
		DeleteDC(hdcMemory);
		hdcMemory=NULL;

		EndPaint(hWnd,&ps);

		BringWindowToTop(hWnd);
		break;

	case WM_DESTROY:
		KillTimer (hWnd, TIMER_SEC) ;
		KillTimer (hWnd, TIMER_MIN) ;
		KillTimer (hWnd, TIMER_FRA) ;

		delete theApp.m_pImageBack;
		theApp.m_pImageBack = NULL;
		GdiplusShutdown(theApp.GetGDIToken());
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**** 导出函数
* 函数功能：外部调用命令初始化气泡窗口
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void OutWindowInit( HWND wnd )
{
	if(!theApp.m_bIsCreate)
	{
		// 设定挂靠窗口的wnd
		theApp.SetFartherWnd(wnd);

		theApp.m_Blend.BlendOp=0;					//theonlyBlendOpdefinedinWindows2000
		theApp.m_Blend.BlendFlags=0;				//nothingelseisspecial...
		theApp.m_Blend.AlphaFormat=1;				//...
		theApp.m_Blend.SourceConstantAlpha=255;		//AC_SRC_ALPHA // 透明度
		theApp.m_pBGMouseIn = Gdiplus::Image::FromFile(L"bgmousein.png");
		theApp.m_pBGMouseOut = Gdiplus::Image::FromFile(L"bgmouseout.png");
		theApp.m_pBGFan = Gdiplus::Image::FromFile(L"bgbanyuan.png");

		theApp.m_pImageBack = theApp.m_pBGMouseOut;

		theApp.m_BakWidth = theApp.m_pImageBack->GetWidth();
		theApp.m_BakHeight = theApp.m_pImageBack->GetHeight();

		// 创建帧刷新线程
		theApp.m_hThreadRec = ::CreateThread(NULL, 0, ThreadFRA, (LPVOID)theApp.GetInstance(), 0, NULL);
	}
}

/**** 导出函数
* 函数功能：显示气泡窗口
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void ShowOutWindow()
{
	ShowWindow(theApp.GetOutWindow(),SW_SHOW);
}

/**** 导出函数
* 函数功能：隐藏气泡窗口
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void HideOutWindow()
{
	ShowWindow(theApp.GetOutWindow(),SW_HIDE);
}

/**** 导出函数
* 函数功能：关闭气泡窗口，释放资源
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void CloseOutWindow()
{
}

/**** 导出函数
* 函数功能：设置位置
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void SetOutWndPos(float x, float y)
{
}

/**** 导出函数
* 函数功能：改变标题
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void SetOutWindowTile(LPCSTR stile)
{
}

/**** 导出函数
* 函数功能：报警
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void OutWindowAlarm()
{
}

/**** 导出函数
* 函数功能：取消报警
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void CancelOutWindowAlarm()
{
}