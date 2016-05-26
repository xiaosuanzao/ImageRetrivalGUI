
// ImageRetrivalDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CImageRetrivalDlg �Ի���



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


// CImageRetrivalDlg ��Ϣ�������

BOOL CImageRetrivalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImageRetrivalDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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
	int rw = rect.right - rect.left;            // ���ͼƬ�ؼ��Ŀ�͸�
	int rh = rect.bottom - rect.top;
	
	IplImage* dstImg = cvCreateImage(cvSize(rw, rh), img->depth, img->nChannels);
	cvResize(img, dstImg);

	CvvImage cimg;
	cimg.CopyOf(dstImg); // ����ͼƬ
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC(pDC);
	cvReleaseImage(&dstImg);
}


void CImageRetrivalDlg::OnBnClickedLoadpicture()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dialog(TRUE);
	if (dialog.DoModal() == IDOK)
	{
		m_ImagePath = dialog.GetPathName();
		UpdateData(FALSE);				
		
		//��ʾԭʼͼ��		
		USES_CONVERSION;
		char* imgPath = T2A(m_ImagePath);
		IplImage *image = cvLoadImage(imgPath);
		DrawPicToHDC(image, IDC_SRCPICTURE);
		cvReleaseImage(&image);		
	}
}


void CImageRetrivalDlg::OnBnClickedRetrival()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_ImagePath.IsEmpty())
	{		
		MessageBox(_T("����ͼƬ·������Ϊ��"), _T("��ʾ"));
		return;
	}
	USES_CONVERSION;
	std::string retrivePath(W2A(m_ImagePath));

	cv::Mat image = retrival.showResult(retrival.retrive(retrivePath));

	DrawPicToHDC(&IplImage(image), IDC_RESULTPICTURE);
	
}
