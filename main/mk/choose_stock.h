#pragma once


// choose_stock 对话框

class choose_stock : public CDialog
{
	DECLARE_DYNAMIC(choose_stock)

public:
	/*界面控件变量*/
	CTabCtrl *CTab;
	/*第一界面的部分控件*/
	CWnd *pTab1_group[6];   //用指针数组来存放第一界面的所有控件
	CListBox *CList1;
	CListBox *CList2;
	/*第二界面的部分控件*/
	CWnd *pTab2_group[4]; 
	CEdit *CEdit2;
	CEdit *CEdit3;




	choose_stock(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~choose_stock();

// 对话框数据
	enum { IDD = IDD_CHOOSE_STOCK };

protected:
	void Tab_hide_show(CWnd *pWnd,int nSize,int nState);  //tab页控件的显示与隐藏

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedOk();
};
