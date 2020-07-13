// SharkDllMenuDialog.cpp: 实现文件
//

#include "pch.h"
#include "SharkDll.h"
#include "SharkDllMenuDialog.h"
#include "afxdialogex.h"
#include "hook.h"
#include "module.h"



void DlgThread(HMODULE hInstance);

//我们计算得到的相对地址，通过获得WeChatWin.dll的基址就可以得到本次运行的虚拟地址
//调用关键call的相对地址
#define REVOCK_CALL_RVA 0x52d358
//关键call的相对地址
#define REVOCK_CALL_TARGET_RVA 0x11d6f9a

SharkDllMenuDialog* sharkDialog;

//关键call调用时的地址
DWORD revockCallVA = 0;
//原来的函数调用
//55EFD358    E8 3D9CCA00     call WeChatWi.56BA6F9A
DWORD revockCallTargetVA = 0;
//原来的函数调用的下一句,调回的一句
//55EFD35D    8B07            mov eax,dword ptr ds:[edi]
DWORD revockCallJmpBackVA = 0;

DWORD wechatWinAddr = 0;
BYTE backCode[5];

HWND m_dialog_hwnd;

// SharkDllMenuDialog 对话框

IMPLEMENT_DYNAMIC(SharkDllMenuDialog, CDialogEx)

SharkDllMenuDialog::SharkDllMenuDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_vxinfo(_T(""))
{
	sharkDialog = this;
}


SharkDllMenuDialog::~SharkDllMenuDialog()
{
}

void SharkDllMenuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_vxinfo);
}


BEGIN_MESSAGE_MAP(SharkDllMenuDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &SharkDllMenuDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &SharkDllMenuDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// SharkDllMenuDialog 消息处理程序
CEdit* pBoxOne;

void SharkDllMenuDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	// 开启消息防撤回

	CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(DlgThread), 0, 0, NULL);
	//将编辑框保存到pBoxOne
	pBoxOne = (CEdit*)sharkDialog->GetDlgItem(IDC_EDIT1);
}

//定义用于连接的字符串
CString back_msg = NULL;
CString back_split = CString(L"\r\n");
CString back_msginfo = CString(L"撤回消息为:");
CString back_tips = NULL;



void OnRevock(DWORD esp) {
	wchar_t *tips = *(wchar_t **)(esp + 0x4);
	wchar_t *msg = *(wchar_t **)(esp + 0x0);
	if (NULL != tips) {
		//WCHAR buffer[0x8192];
		char buff2[0x8192];
		//因为hook的位置会有多次调用，所以需要判断tips和msg是否是我们需要的关键信息
		wchar_t* pos = wcsstr(tips, L"<revokemsg>");
		wchar_t* pos2 = wcsstr(msg, L"<revokemsg>");
		//如果需要的话我们把撤回的消息显示在编辑框中
		if (pos2==NULL && pos != NULL && NULL != msg) {
			CString temp_tip(tips);
			CString temp_msg(msg);

			//排除空信息
			if (temp_msg.IsEmpty() || temp_msg.GetLength() == 0 || !temp_msg) {
				return;
			}
			//排除重复的信息
			if (back_msg == temp_msg && back_tips == temp_tip) {
				return;
			}
			//下面就是以下字符串处理了
			back_msg = CString(msg);
			back_tips = CString(tips);

			//设置信息
			CString last_result;
			pBoxOne->GetWindowText(last_result);
			last_result = last_result + back_tips + back_split + back_msginfo + back_msg + back_split;

			pBoxOne->SetWindowText(last_result);

		}
	}

}
DWORD tEsp = 0;
_declspec(naked) void _OnRevock() {
	//调用我们的业务函数前，请先保存现场
	__asm {
		mov tEsp, esp
		pushad
	}
	OnRevock(tEsp);
	//以下是调用原来的函数，执行正常的流程
	__asm {
		popad
		call revockCallTargetVA
		jmp revockCallJmpBackVA
	}
}


//开启线程进行hook，不开线程会卡死
void DlgThread(HMODULE hInstance) {
	//hook开始
	MessageBoxA(NULL, "DLL Attached!\n", "Game Hacking", MB_OK | MB_TOPMOST);

	//得到WeChatWin.dll的基址
	wechatWinAddr = GetWxModuleAddress();
	//计算出关键call调用时的地址
	revockCallVA = wechatWinAddr + REVOCK_CALL_RVA;
	//计算出关键call的地址
	revockCallTargetVA = wechatWinAddr + REVOCK_CALL_TARGET_RVA;
	//指向了 55EFD35D    8B07            mov eax,dword ptr ds:[edi]
	//这里是通关计算得到调用call后的下一条指令，并没有使用到我们上一章的相对地址，因为上条指令刚好是5个字节所以+5
	revockCallJmpBackVA = revockCallVA + 5;
	StartHook5(wechatWinAddr + REVOCK_CALL_RVA, backCode, _OnRevock);

}


void SharkDllMenuDialog::OnBnClickedButton1()
{
	// 清除编辑框
	sharkDialog->m_vxinfo = "";
	sharkDialog->UpdateData(false);
}

