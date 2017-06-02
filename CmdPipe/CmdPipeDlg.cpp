
// CmdPipeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CmdPipe.h"
#include "CmdPipeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCmdPipeDlg 对话框



CCmdPipeDlg::CCmdPipeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CMDPIPE_DIALOG, pParent)
	, m_strCmd(_T(""))
	, m_strValue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCmdPipeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strCmd);
	DDX_Text(pDX, IDC_EDIT2, m_strValue);
}

BEGIN_MESSAGE_MAP(CCmdPipeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCmdPipeDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCmdPipeDlg 消息处理程序

BOOL CCmdPipeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SECURITY_ATTRIBUTES  sa = { 0 };
	STARTUPINFO          si = { 0 };
	PROCESS_INFORMATION  pi = { 0 };
	char strShellPath[MAX_PATH];
	ZeroMemory(strShellPath, MAX_PATH);

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	//创建管道
	if (!CreatePipe(&m_hReadPipeHandle, &m_hWritePipeShell, &sa, 0))
	{
		if (m_hReadPipeHandle != NULL)	CloseHandle(m_hReadPipeHandle);
		if (m_hWritePipeShell != NULL)	CloseHandle(m_hWritePipeShell);
		return FALSE;
	}

	if (!CreatePipe(&m_hReadPipeShell, &m_hWritePipeHandle, &sa, 0))
	{
		if (m_hWritePipeHandle != NULL)	CloseHandle(m_hWritePipeHandle);
		if (m_hReadPipeShell != NULL)	CloseHandle(m_hReadPipeShell);
		return FALSE;
	}

	memset((void *)&si, 0, sizeof(si));
	memset((void *)&pi, 0, sizeof(pi));

	GetStartupInfo(&si);
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdInput = m_hReadPipeShell;       //这里指定cmd的输出管道
	si.hStdOutput = si.hStdError = m_hWritePipeShell;   //这里指定cmd输入管道

	GetSystemDirectory(strShellPath, MAX_PATH);
	strcat_s(strShellPath, "\\cmd.exe");

	//用si  参数创建进程  就是说指定了他的输入输出
	if (!CreateProcess(strShellPath, NULL, NULL, NULL, TRUE,
		NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		CloseHandle(m_hReadPipeShell);
		CloseHandle(m_hWritePipeShell);
		return FALSE;
	}
	
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCmdPipeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCmdPipeDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
		

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCmdPipeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCmdPipeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	/*UpdateData(true);
	unsigned long	ByteWrite;
	m_strCmd += "\r\n";
	WriteFile(m_hWritePipeHandle, m_strCmd.GetBuffer(0), m_strCmd.GetLength() + 1, &ByteWrite, NULL);*/
	ReadPipe();
	//CDialogEx::OnOK();
}


void CCmdPipeDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TerminateProcess(m_hProcessHandle, 0);   //关闭打开的cmd进程
	CDialogEx::OnClose();
}


void CCmdPipeDlg::ReadPipe()
{
	
	unsigned long   BytesRead = 0;
	unsigned long   BytesWrite = 0;
	char	ReadBuff[1024];
	int ret;
	DWORD	TotalBytesAvail;
	memset(ReadBuff, 0, sizeof(ReadBuff));
	ret = PeekNamedPipe(m_hReadPipeHandle, ReadBuff, sizeof(ReadBuff), &BytesRead, &TotalBytesAvail, NULL);
	if (BytesRead)
	{

		LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, TotalBytesAvail + 1);
		ReadFile(m_hReadPipeHandle, lpBuffer, TotalBytesAvail, &BytesRead, NULL);
		SetDlgItemText(IDC_EDIT2, (char*)lpBuffer);
		LocalFree(lpBuffer);

	}
	else
	{
		WriteFile(m_hWritePipeHandle, "\r\n", 2, &BytesWrite, 0);
		PeekNamedPipe(m_hReadPipeHandle, ReadBuff, sizeof(ReadBuff), &BytesRead, &TotalBytesAvail, NULL);
		ReadFile(m_hReadPipeHandle,ReadBuff,1024,&BytesRead,0);
		BytesRead = 0;



		UpdateData(true);
		unsigned long	ByteWrite;
		m_strCmd += "\r\n";
		
		WriteFile(m_hWritePipeHandle, m_strCmd.GetBuffer(0), m_strCmd.GetLength(), &ByteWrite, NULL);
		//
		Sleep(100);

		
		memset(ReadBuff, 0, sizeof(ReadBuff));
		PeekNamedPipe(m_hReadPipeHandle, ReadBuff, sizeof(ReadBuff), &BytesRead, &TotalBytesAvail, NULL);
		LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, TotalBytesAvail + 1);
		ReadFile(m_hReadPipeHandle, lpBuffer, TotalBytesAvail, &TotalBytesAvail, NULL);
		SetDlgItemText(IDC_EDIT2, (char*)lpBuffer);
		LocalFree(lpBuffer);


	}
}



void CCmdPipeDlg::InitPipe()
{
	unsigned long   BytesRead = 0;
	unsigned long   BytesWrite = 0;
	char	ReadBuff[1024];
	int ret;
	DWORD	TotalBytesAvail;
	memset(ReadBuff, 0, sizeof(ReadBuff));
	Sleep(100);
	ret = PeekNamedPipe(m_hReadPipeHandle, ReadBuff, sizeof(ReadBuff), &BytesRead, &TotalBytesAvail, NULL);
	if (TotalBytesAvail)
	{
		memset(ReadBuff, 0, sizeof(ReadBuff));
		ReadFile(m_hReadPipeHandle, ReadBuff, TotalBytesAvail, &BytesRead, NULL);
		m_strValue = ReadBuff;
		UpdateData(FALSE);
		
	}
}
