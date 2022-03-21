// CPage1.cpp: 实现文件
//

#include "pch.h"
#include "MFCLibrary1.h"
#include "CPage1.h"
#include "afxdialogex.h"
#include "CCheat.h"
#include "./include/detours.h"

// CPage1 对话框

IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PAGE1, pParent)
{
}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPage1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage1::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK1, &CPage1::OnBnClickedCheck1)
END_MESSAGE_MAP()

// CPage1 消息处理程序

CCheat cheat;
void CPage1::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	cheat.Initialize();
}

void CPage1::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	cheat.Reseale();
}

void CPage1::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	::MessageBox(NULL, TEXT("hello"), TEXT("message"), MB_OK);
}

int (WINAPI* Message)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) = ::MessageBox;

int WINAPI HookMessage(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	lpText = TEXT("HOOK");

	return Message(hWnd, lpText, lpCaption, uType);
}

void CPage1::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttach(&(PVOID&)Message, HookMessage);

		DetourTransactionCommit();
	}
	else
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourDetach(&(PVOID&)Message, HookMessage);

		DetourTransactionCommit();
	}
}