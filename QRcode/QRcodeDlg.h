
// QRcodeDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CQRcodeDlg 对话框
class CQRcodeDlg : public CDialogEx
{
// 构造
public:
	CQRcodeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QRCODE_DIALOG };
#endif

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
	CEdit m_dateTdit;
	CEdit m_currentEdit;
	CEdit m_voltageEdit;
	


	afx_msg void OnBnClickedGenerateQRcode();
	CStatic m_ShowPicture;
};
