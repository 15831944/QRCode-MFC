
// QRcode.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQRcodeApp: 
// �йش����ʵ�֣������ QRcode.cpp
//

class CQRcodeApp : public CWinApp
{
public:
	CQRcodeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQRcodeApp theApp;