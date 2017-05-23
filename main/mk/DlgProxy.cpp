
// DlgProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "mk.h"
#include "DlgProxy.h"
#include "mkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmkDlgAutoProxy

IMPLEMENT_DYNCREATE(CmkDlgAutoProxy, CCmdTarget)

CmkDlgAutoProxy::CmkDlgAutoProxy()
{
	EnableAutomation();
	
	// 为使应用程序在自动化对象处于活动状态时一直保持 
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CmkDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CmkDlg)))
		{
			m_pDialog = reinterpret_cast<CmkDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CmkDlgAutoProxy::~CmkDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CmkDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CmkDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CmkDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_Imk 的支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {71A97981-2CD6-4F8D-BEE2-8599A58725DA}
static const IID IID_Imk =
{ 0x71A97981, 0x2CD6, 0x4F8D, { 0xBE, 0xE2, 0x85, 0x99, 0xA5, 0x87, 0x25, 0xDA } };

BEGIN_INTERFACE_MAP(CmkDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CmkDlgAutoProxy, IID_Imk, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏在此项目的 StdAfx.h 中定义
// {52E15AFB-9124-4592-8253-BB7D67DB6CC6}
IMPLEMENT_OLECREATE2(CmkDlgAutoProxy, "mk.Application", 0x52e15afb, 0x9124, 0x4592, 0x82, 0x53, 0xbb, 0x7d, 0x67, 0xdb, 0x6c, 0xc6)


// CmkDlgAutoProxy 消息处理程序
