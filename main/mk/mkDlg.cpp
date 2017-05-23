
// mkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mk.h"
#include "mkDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include <process.h>
#include "workthread.h"
#include "choose_stock.h"

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
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmkDlg 对话框

BEGIN_DHTML_EVENT_MAP(CmkDlg)

END_DHTML_EVENT_MAP()



IMPLEMENT_DYNAMIC(CmkDlg, CDHtmlDialog);

CmkDlg::CmkDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CmkDlg::IDD, CmkDlg::IDH, pParent)
{

	is_suspend=false;

	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CmkDlg::~CmkDlg()
{
	// 如果该对话框有自动化代理，则
	//  将此代理指向该对话框的后向指针设置为 NULL，以便
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;




}

void CmkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmkDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT2, &CmkDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON1, &CmkDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CmkDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CmkDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CmkDlg 消息处理程序


unsigned __stdcall ReceivingThrd(void * pParam);

BOOL CmkDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//给全局的句柄赋值，其实也可以不用全局句柄，而是将句柄传到工作线程
	
	hedit1=GetDlgItem(IDC_EDIT1)->GetSafeHwnd();
	hedit2=GetDlgItem(IDC_EDIT2)->GetSafeHwnd();
	hedit3=GetDlgItem(IDC_EDIT3)->GetSafeHwnd();


    GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);   //令按钮不可操作





	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CmkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmkDlg::OnPaint()
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
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。





BOOL CmkDlg::CanExit()
{
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void CmkDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDHtmlDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CmkDlg::OnBnClickedButton1()
{
	//SuspendThread(hWorkThread);  //调用SuspendThread时必须小心，因为不知道暂停线程运行时它在进行什么操作。

	if(is_suspend==false)
	{      
		SuspendThread(hWorkThread); 
		is_suspend=true;
		SetDlgItemText(IDC_BUTTON1,"唤醒线程");
	}
	else
	{
		ResumeThread(hWorkThread);
		is_suspend=false;
		SetDlgItemText(IDC_BUTTON1,"暂停线程");
	}


}


//void CmkDlg::OnEnChangeEdit1()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDHtmlDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//}


void CmkDlg::OnBnClickedButton3()
{


				//建立个新线程,也就是主要的工作线程。
			hWorkThread = (HANDLE)_beginthreadex( NULL, 0, &WorkThrd,(void *)0, 0, &WorkThreadID );   

			if(hWorkThread == NULL)
			AfxMessageBox("无法创建工作线程");

			if((SetThreadPriority(hWorkThread,THREAD_PRIORITY_TIME_CRITICAL)!=0) && (GetThreadPriority(hWorkThread) != THREAD_PRIORITY_TIME_CRITICAL))  //非零，如果函数运行成功;否则为0。 
			{
					 AfxMessageBox("无法设置工作线程优先级，退出线程");
			 
			}


	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);   //令按钮不可操作
    GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);   //令按钮不可操作


}


void CmkDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	choose_stock  dlg;
	int nRet=dlg.DoModal();
	if(nRet==IDOK)  //只在按确定按钮返回后才do something
	{
	}
}


//void CmkDlg::OnEnChangeEdit3()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDHtmlDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//}
