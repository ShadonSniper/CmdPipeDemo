
// CmdPipeDlg.h : 头文件
//

#pragma once


// CCmdPipeDlg 对话框
class CCmdPipeDlg : public CDialogEx
{
// 构造
public:
	CCmdPipeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMDPIPE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	HANDLE m_hReadPipeHandle;    //管道句柄
	HANDLE m_hWritePipeHandle;
	HANDLE m_hReadPipeShell;
	HANDLE m_hWritePipeShell;

	HANDLE m_hProcessHandle;        //进程句柄

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strCmd;
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
protected:
	void ReadPipe();
public:
	CString m_strValue;

	void InitPipe();
};
