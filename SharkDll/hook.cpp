#include "pch.h"
#include "hook.h"

/**
* 安装Hook
* hookAddr 内存地址
* backCode 备份的地址
* HookImpl 跳转到的函数，该函数为裸函数
*/
int StartHook5(DWORD hookAddr, BYTE backCode[5], void(*FuncBeCall)()) {
	DWORD jmpAddr = (DWORD)FuncBeCall - (hookAddr + 5);

	BYTE jmpCode[5];
	*(jmpCode + 0) = 0xE9;
	*(DWORD *)(jmpCode + 1) = jmpAddr;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	//备份被替换的
	if (ReadProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 5, NULL) == 0) {
		return -1;
	}
	//写入jmp指令
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, jmpCode, 5, NULL) == 0) {
		return -1;
	}

	return 0;
}

/**
* 安装Hook
* hookAddr 内存地址
* backCode 备份的地址
* HookImpl 跳转到的函数，该函数为裸函数
*/
int StartHook6(DWORD hookAddr, BYTE backCode[6], void(*FuncBeCall)()) {
	DWORD jmpAddr = (DWORD)FuncBeCall - (hookAddr + 5);

	BYTE jmpCode[5];
	*(jmpCode + 0) = 0xE9;
	*(DWORD *)(jmpCode + 1) = jmpAddr;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	//备份call指令
	if (ReadProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 6, NULL) == 0) {
		return -1;
	}
	//写入jmp指令
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, jmpCode, 5, NULL) == 0) {
		return -1;
	}
	BYTE nopCode[1];
	nopCode[0] = (BYTE)0x90;
	//写入nop指令
	if (WriteProcessMemory(hProcess, (LPVOID)(hookAddr + 5), nopCode, 1, NULL) == 0) {
		return -1;
	}

	return 0;
}

/**
* 卸载Hook
* hookAddr 内存地址
* backCode 备份的地址
*/
int Unhook5(DWORD hookAddr, BYTE backCode[5]) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 5, NULL) == 0) {
		return -1;
	}
	return 0;
}
/**
* 卸载Hook
* hookAddr 内存地址
* backCode 备份的地址
*/
int Unhook6(DWORD hookAddr, BYTE backCode[6]) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 6, NULL) == 0) {
		return -1;
	}
	return 0;
}
