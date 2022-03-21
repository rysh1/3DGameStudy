#pragma once
#include "CPage1.h"
#include "CPage2.h"
#include "TabSheet.h"

// CMainView 对话框

class CMainView : public CDialog
{
	DECLARE_DYNAMIC(CMainView)

public:
	CMainView(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMainView();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CPage2 m_Page2;
	CPage1 m_Page1;
	CTabSheet m_ctrTab;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
