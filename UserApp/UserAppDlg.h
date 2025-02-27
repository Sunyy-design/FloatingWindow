
// UserAppDlg.h : 头文件
//

#pragma once


// CUserAppDlg 对话框
class CUserAppDlg : public CDialogEx
{
// 构造
public:
	CUserAppDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_USERAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedShow();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedHide2();
};
