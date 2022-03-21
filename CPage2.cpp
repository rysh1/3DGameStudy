// CPage2.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary1.h"
#include "CPage2.h"
#include "afxdialogex.h"

// CPage2 对话框

IMPLEMENT_DYNAMIC(CPage2, CDialog)

CPage2::CPage2(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PAGE2, pParent)
{
}

CPage2::~CPage2()
{
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPage2, CDialog)
END_MESSAGE_MAP()

// CPage2 消息处理程序