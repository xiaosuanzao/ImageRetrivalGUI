
// ImageRetrivalDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CvvImage.h"
#include "Retrival.h"


// CImageRetrivalDlg 对话框
class CImageRetrivalDlg : public CDialogEx
{
// 构造
public:
	CImageRetrivalDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IMAGERETRIVAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	
	DECLARE_MESSAGE_MAP()

private:
	void DrawPicToHDC(IplImage *img, UINT ID);


public:
		 
	CString m_ImagePath;
	afx_msg void OnBnClickedLoadpicture();
	afx_msg void OnBnClickedRetrival();

private:

	Retrival retrival;
};
