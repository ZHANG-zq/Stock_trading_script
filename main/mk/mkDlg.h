
// mkDlg.h : 头文件
//

#pragma once

class CmkDlgAutoProxy;


// CmkDlg 对话框
class CmkDlg : public CDHtmlDialog
{
	DECLARE_DYNAMIC(CmkDlg);
	friend class CmkDlgAutoProxy;

// 构造
public:
	CmkDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CmkDlg();


	
	//工作线程句柄与ID
	HANDLE hWorkThread;
    unsigned WorkThreadID;
	bool is_suspend; //控制暂停，唤醒



// 对话框数据
	enum { IDD = IDD_MK_DIALOG, IDH = IDR_HTML_MK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CmkDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
//	afx_msg void OnEnChangeEdit3();
};
