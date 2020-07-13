#pragma once

#include "pch.h"
#include <Windows.h>
int StartHook5(DWORD hookAddr, BYTE backCode[5], void(*FuncBeCall)());
int Unhook5(DWORD hookAddr, BYTE backCode[5]);

int StartHook6(DWORD hookAddr, BYTE backCode[6], void(*FuncBeCall)());
int Unhook6(DWORD hookAddr, BYTE backCode[6]);