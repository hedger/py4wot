#pragma once

#include "stdafx.h"

namespace Tools
{
  DWORD GetModulePath(HINSTANCE hInst, char* pszBuffer, DWORD dwSize);
  void UnloadDLL(HMODULE hModule);
  DWORD GetModuleSize(HMODULE hMod);
  BOOL DataCompare(const char* pData, const char* bMask, const char * szMask);
  DWORD FindPattern(DWORD dwAddress, DWORD dwLen, const char *bMask, const char* szMask);
  DWORD FindPattern(const char* pDllName, const char* pattern, const char* mask);
}