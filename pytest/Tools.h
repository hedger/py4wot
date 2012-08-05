/* [APB:Reloaded] Hack by Hedger & h0pk1ns */

#pragma once

#include "stdafx.h"
#include <Tlhelp32.h>
#include <stdio.h>
#include <string>

namespace Tools
{
  extern std::wstring ModuleDir;
  extern WCHAR FilePath[MAX_PATH]; // File (Config) Path

  const wchar_t* GetFilePath(HINSTANCE hInstance, const WCHAR* Ext);
  void UnloadDLL(HMODULE hModule);
  void DebugLog(FILE* File, const char *Msg, ...);
  DWORD GetModuleSize(HMODULE hMod);
  BOOL DataCompare(const char* pData, const char* bMask, const char * szMask);
  DWORD FindPattern(DWORD dwAddress, DWORD dwLen, const char *bMask, const char* szMask);
  DWORD FindPattern(const char* pDllName, const char* pattern, const char* mask);
}