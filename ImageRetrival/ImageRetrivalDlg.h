
// ImageRetrivalDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "CvvImage.h"
#include "Retrival.h"


// CImageRetrivalDlg �Ի���
class CImageRetrivalDlg : public CDialogEx
{
// ����
public:
	CImageRetrivalDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IMAGERETRIVAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
