
// CmdPipeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CmdPipe.h"
#include "CmdPipeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCmdPipeDlg �Ի���



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


// CCmdPipeDlg ��Ϣ�������

BOOL CCmdPipeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SECURITY_ATTRIBUTES  sa = { 0 };
	STARTUPINFO          si = { 0 };
	PROCESS_INFORMATION  pi = { 0 };
	char strShellPath[MAX_PATH];
	ZeroMemory(strShellPath, MAX_PATH);

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	//�����ܵ�
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
	si.hStdInput = m_hReadPipeShell;       //����ָ��cmd������ܵ�
	si.hStdOutput = si.hStdError = m_hWritePipeShell;   //����ָ��cmd����ܵ�

	GetSystemDirectory(strShellPath, MAX_PATH);
	strcat_s(strShellPath, "\\cmd.exe");

	//��si  ������������  ����˵ָ���������������
	if (!CreateProcess(strShellPath, NULL, NULL, NULL, TRUE,
		NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		CloseHandle(m_hReadPipeShell);
		CloseHandle(m_hWritePipeShell);
		return FALSE;
	}
	
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCmdPipeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
		

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCmdPipeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCmdPipeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*UpdateData(true);
	unsigned long	ByteWrite;
	m_strCmd += "\r\n";
	WriteFile(m_hWritePipeHandle, m_strCmd.GetBuffer(0), m_strCmd.GetLength() + 1, &ByteWrite, NULL);*/
	ReadPipe();
	//CDialogEx::OnOK();
}


void CCmdPipeDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TerminateProcess(m_hProcessHandle, 0);   //�رմ򿪵�cmd����
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
