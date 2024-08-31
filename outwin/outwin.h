// outwin.h : outwin DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include <comdef.h>
#include "gdiplus.h"

using namespace Gdiplus;

#define TIMER_SEC 1			// �����ʱ��        
#define TIMER_MIN 2			// �����ʱ��
#define TIMER_FRA 3			// ֡ˢ�¼�ʱ��

#define OUTWND_STATE_MOUSEOUT  0		//״̬�����δ����
#define OUTWND_STATE_MOUSEIN  1			//״̬��������
#define OUTWND_STATE_FAN  2				//״̬����������



#ifdef __cplusplus
        
#define EXPORT extern "C" __declspec (dllexport)
        
#else
        
#define EXPORT __declspec (dllexport)
        
#endif

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM MyRegisterClass(HINSTANCE hInstance);
// �ⲿ������Ϣ����
LRESULT CALLBACK OutWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

// ���尴ť��Ӧ������������
typedef void (*BTNCALL)(LPVOID);

// ���ǰ�ť�ص�����
void SBtn_Call(void *p);
// wnd��ť����
void WBtn_Call(void *p);

/**** 
* ��ť��
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
class MButton
{
public:
	MButton();
	MButton(PTCHAR inpath, PTCHAR outpath,int x,int y,bool show,BTNCALL lpBtnCall,void* pAgv);
private:
	Image *m_pmousein;			// �����������ʱ��ʾ��ͼ��
	Image *m_pmouseout;			// ��겻��
	BTNCALL m_lpBtnCall;		// ��ť���ú���
	void* m_lpBtnCallAgv;		// ��ť���ú�������
	int m_nwidth;				// ͼ���
	int m_nheight;				// ͼ���
	int m_nx;					// ��ťposx
	int m_ny;					// ��ťposy

	
	RECT rct;					// ��ť�ж�����

private:
	bool IsMouseOn();	// �жϺ�������Ƿ��ڰ�ť��
public:
	void OnPaint(Graphics *graph);	// ÿ֡�ػ溯��
	void OnLButtonUp();			// ������������

	bool m_bishow;				// �Ƿ���ʾ
};

// CoutwinApp
// �йش���ʵ�ֵ���Ϣ������� outwin.cpp
//

class CoutwinApp : public CWinApp
{
public:
	CoutwinApp();

// ��д
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()

// ˽�ж���
private:
	HINSTANCE m_hInstance;			// ��¼��dll�ĳ�����
	
	HWND m_OutWindow;				// ��¼�ⲿ����wnd
	HWND m_hFartherWnd;				// ��¼���ര��wnd

	// GDI+��ʼ������
	GdiplusStartupInput   m_gdiplusStartupInput; 
	ULONG_PTR   m_gdiplusToken;
public:
	BLENDFUNCTION m_Blend;				// ���δ��ڲ���
	Image *m_pImageBack;				// ����ͼ��ָ�룬Image��һ��ͼ���ࡣ
	Image *m_pBGMouseIn;
	Image *m_pBGMouseOut;
	Image *m_pAlaram;
	Image *m_pBGFan;						// ������屳��
	int m_BakWidth;						// ͼ����
	int m_BakHeight;
	HANDLE m_hThreadRec;			// ֡ˢ���߳�

	int m_noutwndstate;				// �ⲿ����״̬��
	int mousex,mousey;					// ������ⲿ�����ϵ�����

	LPCWSTR m_sbig;					// ��Բ����ʾ������
	LPCWSTR m_sup;					// �ϼ�ͷ�������
	LPCWSTR m_sdown;					// �¼�ͷ�������

public:
	void SetFartherWnd( HWND fwnd ){ m_hFartherWnd = fwnd; }	
	HINSTANCE GetInstance(){return m_hInstance;};
	ULONG_PTR GetGDIToken(){ return m_gdiplusToken; }
	HWND GetOutWindow(){return m_OutWindow; }
	void SetOutWindow( HWND wnd );
	void OnMePaint( HDC hdc,Graphics *gra , Point *p);
	bool m_bIsCreate;							// ��¼�����Ƿ񴴽�

	void OnMELButtonUp();

	MButton *m_pSBtn;							// ������btn
	MButton *m_pWBtn;							// win��btn
};

CoutwinApp* GetApp();

/**** ��������
* �������ܣ��ⲿ���������ʼ�����ݴ���
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void OutWindowInit( HWND wnd );

/**** ��������
* �������ܣ���ʾ���ݴ���
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void ShowOutWindow();

/**** ��������
* �������ܣ��������ݴ���
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void HideOutWindow();

/**** ��������
* �������ܣ��ر����ݴ��ڣ��ͷ���Դ
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void CloseOutWindow();

/**** ��������
* �������ܣ�����λ��
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void SetOutWndPos(float x, float y);

/**** ��������
* �������ܣ��ı����
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void SetOutWindowTile(LPCSTR stile);

/**** ��������
* �������ܣ�����
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void OutWindowAlarm();

/**** ��������
* �������ܣ�ȡ������
* Copyright(C)2013 RabbitSoft Studio
* 2014-10-22 by SaCha
*/
EXPORT void CancelOutWindowAlarm();
