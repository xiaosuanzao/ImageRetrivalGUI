
// ImageRetrivalDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageRetrival.h"
#include "ImageRetrivalDlg.h"
#include "afxdialogex.h"
#include <opencv2\imgproc.hpp>
#include <string>
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageRetrivalDlg 对话框



CImageRetrivalDlg::CImageRetrivalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageRetrivalDlg::IDD, pParent)
	, m_ImagePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageRetrivalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_PATH, m_ImagePath);
}

BEGIN_MESSAGE_MAP(CImageRetrivalDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_BN_CLICKED(IDC_LOADPICTURE, &CImageRetrivalDlg::OnBnClickedLoadpicture)
	ON_BN_CLICKED(IDC_RETRIVAL, &CImageRetrivalDlg::OnBnClickedRetrival)
END_MESSAGE_MAP()


// CImageRetrivalDlg 消息处理程序

BOOL CImageRetrivalDlg::OnInitDialog()
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

	// TODO:  在此添加额外的初始化代码	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImageRetrivalDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImageRetrivalDlg::OnPaint()
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

void CImageRetrivalDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;            // 求出图片控件的宽和高
	int rh = rect.bottom - rect.top;
	
	IplImage* dstImg = cvCreateImage(cvSize(rw, rh), img->depth, img->nChannels);
	cvResize(img, dstImg);

	CvvImage cimg;
	cimg.CopyOf(dstImg); // 复制图片
	cimg.DrawToHDC(hDC, &rect); // 将图片绘制到显示控件的指定区域内
	ReleaseDC(pDC);
	cvReleaseImage(&dstImg);
}


void CImageRetrivalDlg::OnBnClickedLoadpicture()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dialog(TRUE);
	if (dialog.DoModal() == IDOK)
	{
		m_ImagePath = dialog.GetPathName();
		UpdateData(FALSE);				
		
		//显示原始图像		
		USES_CONVERSION;
		char* imgPath = T2A(m_ImagePath);
		IplImage *image = cvLoadImage(imgPath);
		DrawPicToHDC(image, IDC_SRCPICTURE);
		cvReleaseImage(&image);		
	}
}


void CImageRetrivalDlg::OnBnClickedRetrival()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_ImagePath.IsEmpty())
	{		
		MessageBox(_T("输入图片路径不能为空"), _T("提示"));
		return;
	}
	USES_CONVERSION;
	std::string retrivePath(W2A(m_ImagePath));

	cv::Mat image = retrival.showResult(retrival.retrive(retrivePath));

	DrawPicToHDC(&IplImage(image), IDC_RESULTPICTURE);
	
}
