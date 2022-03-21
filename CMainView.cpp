// CMainView.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary1.h"
#include "CMainView.h"
#include "afxdialogex.h"

// CMainView 对话框

IMPLEMENT_DYNAMIC(CMainView, CDialog)

CMainView::CMainView(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAINDIALOG, pParent)
{
}

CMainView::~CMainView()
{
}

void CMainView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAINTAB, m_ctrTab);
}

BOOL CMainView::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ctrTab.AddPage(TEXT("test1"), &m_Page1, IDD_PAGE1);
	m_ctrTab.AddPage(TEXT("test2"), &m_Page2, IDD_PAGE2);
	m_ctrTab.Show();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CMainView, CDialog)
END_MESSAGE_MAP()

// CMainView 消息处理程序