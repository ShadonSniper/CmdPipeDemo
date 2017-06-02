
// CmdPipeDlg.h : ͷ�ļ�
//

#pragma once


// CCmdPipeDlg �Ի���
class CCmdPipeDlg : public CDialogEx
{
// ����
public:
	CCmdPipeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMDPIPE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	HANDLE m_hReadPipeHandle;    //�ܵ����
	HANDLE m_hWritePipeHandle;
	HANDLE m_hReadPipeShell;
	HANDLE m_hWritePipeShell;

	HANDLE m_hProcessHandle;        //���̾��

	// ���ɵ���Ϣӳ�亯��
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
