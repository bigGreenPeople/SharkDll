#pragma once
#pragma once
#include <Windows.h>
inline DWORD GetWxModuleAddress() {
	return (DWORD)LoadLibraryW(L"WeChatWin.dll");
}

inline DWORD GetUser32Address() {
	return (DWORD)LoadLibraryW(L"user32.dll");
}