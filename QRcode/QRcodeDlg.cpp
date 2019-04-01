
// QRcodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QRcode.h"
#include "QRcodeDlg.h"
#include "afxdialogex.h"
#include "qrencode.h"

#pragma comment(lib, "qrencode.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CQRcodeDlg 对话框



CQRcodeDlg::CQRcodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QRCODE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQRcodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_dateTdit);
	DDX_Control(pDX, IDC_EDIT2, m_currentEdit);
	DDX_Control(pDX, IDC_EDIT3, m_voltageEdit);
	DDX_Control(pDX, IDC_SHOW_PICTURE, m_ShowPicture);
}

BEGIN_MESSAGE_MAP(CQRcodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CQRcodeDlg::OnBnClickedGenerateQRcode)
END_MESSAGE_MAP()


// CQRcodeDlg 消息处理程序

BOOL CQRcodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQRcodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQRcodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQRcodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CQRcodeDlg::OnBnClickedGenerateQRcode()
{
	//get the text of the edit
	CString dateCString;
	CString currentCString;
	CString voltageCString;
	m_dateTdit.GetWindowText(dateCString);
	m_currentEdit.GetWindowText(currentCString);
	m_voltageEdit.GetWindowText(voltageCString);

	//generate QRcode

	CString dataCString = dateCString + L"/" + currentCString + L"/" + voltageCString;
	USES_CONVERSION;
	char* data = T2A(dataCString);

	//char* data = "2019/03/25/9.0A/220V";
	unsigned int unWidth, unWidthAdjusted, unDataBytes;
	unsigned char* pRBGData, *pSourceDate, *pDestData;
	QRcode* pQRC;
	FILE* f;

	if (pQRC = QRcode_encodeString(data, 1, QR_ECLEVEL_L, QR_MODE_8, 2)) {
		unWidth = pQRC->width;
		unWidthAdjusted = unWidth * 8 * 3;
		if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
		unDataBytes = unWidthAdjusted * unWidth * 8;

		//Allocate pixels buffer

		if (!(pRBGData = (unsigned char*)malloc(unDataBytes))) {
			exit(-1);
		}

		//Preset to white

		memset(pRBGData, 0xff, unDataBytes);

		//Prepare bmp headers
		BITMAPFILEHEADER kFileHeader;
		kFileHeader.bfType = 0x4d42;
		kFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + unDataBytes;
		kFileHeader.bfReserved1 = 0;
		kFileHeader.bfReserved2 = 0;
		kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		BITMAPINFOHEADER kInfoHeader;
		kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		kInfoHeader.biWidth = unWidth * 8;
		kInfoHeader.biHeight = -((int)unWidth * 8);
		kInfoHeader.biPlanes = 1;
		kInfoHeader.biBitCount = 24;
		kInfoHeader.biCompression = BI_RGB;
		kInfoHeader.biSizeImage = 0;
		kInfoHeader.biXPelsPerMeter = 0;
		kInfoHeader.biYPelsPerMeter = 0;
		kInfoHeader.biClrUsed = 0;
		kInfoHeader.biClrImportant = 0;

		//Convert QRCode bits to pixels;

		pSourceDate = pQRC->data;
		for (unsigned int y = 0; y < unWidth; y++) {
			pDestData = pRBGData + unWidthAdjusted * y * 8;
			for (unsigned int x = 0; x < unWidth; x++) {
				if (*pSourceDate & 1) {
					for (unsigned int l = 0; l < 8; l++) {
						for (unsigned int n = 0; n < 8; n++) {
							*(pDestData + n * 3 + unWidthAdjusted * l) = 0;
							*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = 0;
							*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = 0;
						}
					}
				}
				pDestData += 3 * 8;
				pSourceDate++;
			}
		}

		//show the QRcode

		BITMAPINFO bmi;
		bmi.bmiHeader = kInfoHeader;
		CDC* dc;
		dc = GetDlgItem(IDC_SHOW_PICTURE)->GetDC();
		CRect rc;
		CWnd* pWnd = GetDlgItem(IDC_SHOW_PICTURE);
		pWnd->GetClientRect(&rc);
		::SetDIBitsToDevice(dc->GetSafeHdc(), 0, 0, unWidth * 8, unWidth * 8, 0, 0, 0, unWidth * 8,
			pRBGData, &bmi, DIB_RGB_COLORS);

		// output the bmp file

		if (!(fopen_s(&f, "F:\\temp.bmp", "w+b"))) {
			fwrite(&kFileHeader, 1,  sizeof(BITMAPFILEHEADER), f);
			fwrite(&kInfoHeader, 1,  sizeof(BITMAPINFOHEADER), f);
			fwrite(pRBGData, sizeof(unsigned char), unDataBytes, f);

			fclose(f);
		}
		else {
			MessageBox(L"Unable to open file");
			exit(-1);
		}
		free(pRBGData);
		QRcode_free(pQRC);

	}
	else {
		MessageBox(L"NULL returned");
		exit(-1);
	}
}