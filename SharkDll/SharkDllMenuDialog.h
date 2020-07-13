#pragma once


// SharkDllMenuDialog 对话框

class SharkDllMenuDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SharkDllMenuDialog)

public:
	SharkDllMenuDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SharkDllMenuDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_vxinfo;
	afx_msg void OnBnClickedButton1();
};
