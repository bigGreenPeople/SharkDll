#include "pch.h"
#include "hook.h"

/**
* ��װHook
* hookAddr �ڴ��ַ
* backCode ���ݵĵ�ַ
* HookImpl ��ת���ĺ������ú���Ϊ�㺯��
*/
int StartHook5(DWORD hookAddr, BYTE backCode[5], void(*FuncBeCall)()) {
	DWORD jmpAddr = (DWORD)FuncBeCall - (hookAddr + 5);

	BYTE jmpCode[5];
	*(jmpCode + 0) = 0xE9;
	*(DWORD *)(jmpCode + 1) = jmpAddr;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	//���ݱ��滻��
	if (ReadProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 5, NULL) == 0) {
		return -1;
	}
	//д��jmpָ��
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, jmpCode, 5, NULL) == 0) {
		return -1;
	}

	return 0;
}

/**
* ��װHook
* hookAddr �ڴ��ַ
* backCode ���ݵĵ�ַ
* HookImpl ��ת���ĺ������ú���Ϊ�㺯��
*/
int StartHook6(DWORD hookAddr, BYTE backCode[6], void(*FuncBeCall)()) {
	DWORD jmpAddr = (DWORD)FuncBeCall - (hookAddr + 5);

	BYTE jmpCode[5];
	*(jmpCode + 0) = 0xE9;
	*(DWORD *)(jmpCode + 1) = jmpAddr;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	//����callָ��
	if (ReadProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 6, NULL) == 0) {
		return -1;
	}
	//д��jmpָ��
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, jmpCode, 5, NULL) == 0) {
		return -1;
	}
	BYTE nopCode[1];
	nopCode[0] = (BYTE)0x90;
	//д��nopָ��
	if (WriteProcessMemory(hProcess, (LPVOID)(hookAddr + 5), nopCode, 1, NULL) == 0) {
		return -1;
	}

	return 0;
}

/**
* ж��Hook
* hookAddr �ڴ��ַ
* backCode ���ݵĵ�ַ
*/
int Unhook5(DWORD hookAddr, BYTE backCode[5]) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 5, NULL) == 0) {
		return -1;
	}
	return 0;
}
/**
* ж��Hook
* hookAddr �ڴ��ַ
* backCode ���ݵĵ�ַ
*/
int Unhook6(DWORD hookAddr, BYTE backCode[6]) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	if (WriteProcessMemory(hProcess, (LPVOID)hookAddr, backCode, 6, NULL) == 0) {
		return -1;
	}
	return 0;
}
