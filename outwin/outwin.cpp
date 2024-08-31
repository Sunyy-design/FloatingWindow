// outwin.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "outwin.h"

using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")


#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CoutwinApp

BEGIN_MESSAGE_MAP(CoutwinApp, CWinApp)
END_MESSAGE_MAP()

// CoutwinApp ����

CoutwinApp::CoutwinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	m_noutwndstate = OUTWND_STATE_MOUSEOUT;
	m_pSBtn = NULL;
	m_sbig = L"90%\n";
	m_sup = L"85k/s\n";
	m_sdown = L"3B/s\n";
}


// Ψһ��һ�� CoutwinApp ����

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

	// ��ť��ռ����
	rct.left = m_nx;
	rct.right = m_nx+m_nwidth;
	rct.top = m_ny;
	rct.bottom = m_ny+m_nheight;

	m_bishow = show;

	m_lpBtnCall = lpBtnCall;
	m_lpBtnCallAgv = pAgv;
}

// �ж�����Ƿ��ڰ�ť��
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
	// ��ť�Ƿ���ʾ
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
	// ��ť�Ƿ���ʾ
	if(m_bishow)
	{
		//����Ƿ���������
		if(IsMouseOn())
		{
			// ���ð�ť����ĺ���
			m_lpBtnCall(m_lpBtnCallAgv);
		}
	}
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hWnd  = CreateWindowEx( WS_EX_WINDOWEDGE
		,L"OutWindow"
		,TEXT("���ݴ���")
		,WS_OVERLAPPEDWINDOW | WS_SYSMENU| WS_THICKFRAME
		,0,0,0,0
		,NULL,NULL,hInstance,NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	theApp.SetOutWindow(hWnd);
 
	// ������С�� ��󻯰�ť
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

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, SW_SHOW))
	{
		return FALSE;
	}

	// ����Ϣѭ��:
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


// CoutwinApp ��ʼ��

BOOL CoutwinApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ������ OLE ������(����)ע��Ϊ���С��⽫ʹ
	//  OLE ����Դ�����Ӧ�ó��򴴽�����
	COleObjectFactory::RegisterAll();

	// GDI+������ʼ��
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	// ��ñ�����ľ��
	m_hInstance = AfxGetInstanceHandle();

	m_bIsCreate = false;

	return TRUE;
}

// ���ǰ�ť�ص�����
void SBtn_Call(void *p)
{
	theApp.m_noutwndstate = OUTWND_STATE_FAN;
	return;
}

// ���ǰ�ť�ص�����
void WBtn_Call(void *p)
{
	MessageBox(theApp.GetOutWindow(),L"�۹�����",NULL,MB_OK);
	return;
}

// �趨wnd���� ��ʱ�ѳ�ʼ������Ҳ��������
void CoutwinApp::SetOutWindow( HWND wnd )
{
	m_OutWindow = wnd;
	m_bIsCreate = true;
	// ��ʼ�����ǰ�ť
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
	// ��ť����
	m_pSBtn->OnPaint(graph);
	m_pWBtn->OnPaint(graph);
	// ���ֻ���,����ʹ��ͼƬ������ʾ��ʽ
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

// DllGetClassObject - �����๤��

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}


// DllCanUnloadNow - ���� COM ж�� DLL

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return SELFREG_E_TYPELIB;

	if (!COleObjectFactory::UpdateRegistryAll())
		return SELFREG_E_CLASS;

	return S_OK;
}


// DllUnregisterServer - �����ϵͳע������Ƴ�

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
* �ⲿ���촰����Ϣ����
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

	// TODO: �ڴ���������ͼ����...	

	switch (message) {
	case WM_CREATE:
		// ������������
		SetWindowLong(hWnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW|WS_EX_STATICEDGE);

		// �̶����ڴ�С
		lStyle= GetWindowLong(hWnd,GWL_STYLE);
		lStyle&=~WS_THICKFRAME;
		lStyle&=~WS_MAXIMIZEBOX;
		lStyle&=~WS_MINIMIZEBOX;
		lStyle&=~WS_CAPTION;
		((LPCREATESTRUCT)lParam)->cx=50;
		((LPCREATESTRUCT)lParam)->cy=100;
		SetWindowLong(hWnd,GWL_STYLE,lStyle);

		// �趨���������ϲ�
		SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

		SetTimer (hWnd, TIMER_FRA, 30, NULL) ;
		SetTimer (hWnd, TIMER_SEC, 1000, NULL) ;        
		SetTimer (hWnd, TIMER_MIN, 60000, NULL) ;
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_SEC:
			//ÿ��һ�εĴ���
			break ;
		case TIMER_MIN:
			//ÿ����һ�εĴ���
			break ;
		case TIMER_FRA:
			//ÿ֡һ�εĴ���
			InvalidateRect(hWnd,NULL,true);
			break ;
		}
		break;
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);							// ������겶��(��ֹ����ܳ�����ʧȥ����ȵ�)
		GetCursorPos(&pt);							// ��ȡ�����ָ�뵱ǰλ��
		GetWindowRect(hWnd,&rt);					// ��ȡ����λ�����С
		re.right=rt.right-rt.left;					// ���洰�ڿ��
		re.bottom=rt.bottom-rt.top;					// ���洰�ڸ߶�
		break;
	case WM_MOUSEMOVE:
		GetCursorPos(&pe);											// ��ȡ���ָ�����λ��
		if(wParam==MK_LBUTTON)										// ������Ҽ�����
		{
			re.left=rt.left+(pe.x - pt.x);							// �����µ�ˮƽλ��
			re.top =rt.top+(pe.y - pt.y);							// �����µĴ�ֱλ��
			MoveWindow(hWnd,re.left,re.top,re.right,re.bottom,true);// �ƶ�����
		}
		// ����ϵͳ�� ���ͣ�� �� ����뿪��Ϣ
		if (_bMouseTrack)     // ������ ׷�٣��� 
		{
			TRACKMOUSEEVENT csTME;
			csTME.cbSize = sizeof (csTME);
			csTME.dwFlags = TME_LEAVE|TME_HOVER;
			csTME.hwndTrack = hWnd ;				// ָ��Ҫ ׷�� �Ĵ��� 
			csTME.dwHoverTime = 10;					// ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
			::_TrackMouseEvent (&csTME);			// ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧��

			_bMouseTrack=FALSE ;					// ���Ѿ� ׷�� ����ֹͣ ׷�� 
		}
		break;
	case WM_MOUSEHOVER:
		theApp.mousex = LOWORD(lParam);
		theApp.mousey = HIWORD(lParam);
		if( OUTWND_STATE_MOUSEOUT == theApp.m_noutwndstate )
		{
			// ���ͣ����Ϣ
			theApp.m_noutwndstate = OUTWND_STATE_MOUSEIN;
		}
		break;
	case WM_MOUSELEAVE:
		// ����뿪��Ϣ��������WM_MOUSEMOVE�п�������Ч
		theApp.m_noutwndstate = OUTWND_STATE_MOUSEOUT;
		theApp.mousex = 0;
		theApp.mousey = 0;
		break;
	case WM_LBUTTONUP:
		theApp.OnMELButtonUp();
		ReleaseCapture();                           // �ͷ���겶�񣬻ָ�����״̬
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
		//�Զ�����ƴ���
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

		//����Щͼ �ͽ���theapp��
		theApp.OnMePaint(hdcMemory,graph,points);

		//��ӡ
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

/**** ��������
* �������ܣ��ⲿ���������ʼ�����ݴ���
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void OutWindowInit( HWND wnd )
{
	if(!theApp.m_bIsCreate)
	{
		// �趨�ҿ����ڵ�wnd
		theApp.SetFartherWnd(wnd);

		theApp.m_Blend.BlendOp=0;					//theonlyBlendOpdefinedinWindows2000
		theApp.m_Blend.BlendFlags=0;				//nothingelseisspecial...
		theApp.m_Blend.AlphaFormat=1;				//...
		theApp.m_Blend.SourceConstantAlpha=255;		//AC_SRC_ALPHA // ͸����
		theApp.m_pBGMouseIn = Gdiplus::Image::FromFile(L"bgmousein.png");
		theApp.m_pBGMouseOut = Gdiplus::Image::FromFile(L"bgmouseout.png");
		theApp.m_pBGFan = Gdiplus::Image::FromFile(L"bgbanyuan.png");

		theApp.m_pImageBack = theApp.m_pBGMouseOut;

		theApp.m_BakWidth = theApp.m_pImageBack->GetWidth();
		theApp.m_BakHeight = theApp.m_pImageBack->GetHeight();

		// ����֡ˢ���߳�
		theApp.m_hThreadRec = ::CreateThread(NULL, 0, ThreadFRA, (LPVOID)theApp.GetInstance(), 0, NULL);
	}
}

/**** ��������
* �������ܣ���ʾ���ݴ���
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void ShowOutWindow()
{
	ShowWindow(theApp.GetOutWindow(),SW_SHOW);
}

/**** ��������
* �������ܣ��������ݴ���
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void HideOutWindow()
{
	ShowWindow(theApp.GetOutWindow(),SW_HIDE);
}

/**** ��������
* �������ܣ��ر����ݴ��ڣ��ͷ���Դ
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void CloseOutWindow()
{
}

/**** ��������
* �������ܣ�����λ��
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void SetOutWndPos(float x, float y)
{
}

/**** ��������
* �������ܣ��ı����
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void SetOutWindowTile(LPCSTR stile)
{
}

/**** ��������
* �������ܣ�����
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void OutWindowAlarm()
{
}

/**** ��������
* �������ܣ�ȡ������
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void CancelOutWindowAlarm()
{
}