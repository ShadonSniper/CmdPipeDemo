
// CmdPipe.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCmdPipeApp: 
// �йش����ʵ�֣������ CmdPipe.cpp
//

class CCmdPipeApp : public CWinApp
{
public:
	CCmdPipeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCmdPipeApp theApp;