// SharkDll.h: SharkDll DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号
#include "SharkDllMenuDialog.h"

// CSharkDllApp
// 有关此类实现的信息，请参阅 SharkDll.cpp
//

class CSharkDllApp : public CWinApp
{
public:
	CSharkDllApp();
// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
protected:
	HANDLE m_hThread;//线程句柄

};
