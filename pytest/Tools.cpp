/* [APB:Reloaded] Hack by Hedger & h0pk1ns */

#pragma once

#include "stdafx.h"
#include "Tools.h"

namespace Tools
{
  std::wstring ModuleDir;
  WCHAR FilePath[MAX_PATH]; // File (Config) Path

  /// Get File Path (Module Dir)
  const wchar_t* GetFilePath(HINSTANCE hInstance, const WCHAR* Ext)
  {
    WCHAR ModulePath[MAX_PATH];
    GetModuleFileNameW(hInstance, ModulePath, sizeof(ModulePath));

    wcscpy(FilePath, ModulePath);
    ModuleDir = std::wstring(ModulePath);
    ModuleDir.resize(ModuleDir.rfind(L"\\"));

    wcscpy(&FilePath[wcslen(FilePath) - 3], Ext);

    return FilePath;
  } 

  void UnloadDLL(HMODULE hModule)
  {
    LPVOID ExitThreadAddress = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "ExitThread");
    //MessageBox(0, L"Unloading...", L"...", 32);
    _asm
    {
      push hModule
      push ExitThreadAddress
      jmp dword ptr [FreeLibrary]
    }
  }

  void DebugLog(FILE* File, const char *Msg, ...)
  {
    if (File != NULL)
    {
      va_list va_alist;
      char logbuf[256];
      memset(logbuf, 0, 256);

      va_start(va_alist, Msg);
      _vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), Msg, va_alist);
      va_end(va_alist);

      fprintf(File, "%s", logbuf);
      fflush(File);
    }
  }

  DWORD GetModuleSize(HMODULE hMod)
  {
    if (!hMod)
      return 0;

    MODULEENTRY32 lpme = { 0 };
    DWORD dwSize = 0;
    DWORD PID = GetCurrentProcessId();
    BOOL isMod = 0;

    HANDLE hSnapshotModule = CreateToolhelp32Snapshot(0x8, PID);

    if (hSnapshotModule)
    {
      lpme.dwSize = sizeof(lpme);
      isMod = Module32First(hSnapshotModule, &lpme);

      while (isMod)
      {
        if (lpme.hModule == hMod)
        {
          dwSize = lpme.modBaseSize;
          CloseHandle(hSnapshotModule);

          return dwSize;
        }

        isMod = Module32Next(hSnapshotModule, &lpme);
      }
    }

    CloseHandle(hSnapshotModule);

    return 0;
  }

  BOOL DataCompare(const char* pData, const char* bMask, const char * szMask)
  {
    for ( ; *szMask; ++szMask, ++pData, ++bMask)
      if (*szMask == 'x' && *pData != *bMask)
        return FALSE;

    return (*szMask == NULL);
  }

  DWORD FindPattern(DWORD dwAddress, DWORD dwLen, const char *bMask, const char* szMask)
  {
    for (DWORD I = 0; I < dwLen; I++)
      if (DataCompare((const char*)(dwAddress + I), bMask, szMask))
        return (DWORD)(dwAddress + I);

    return 0;
  }

  DWORD FindPattern(const char* pDllName, const char* pattern, const char* mask)
  {
    HMODULE hDll = GetModuleHandleA(pDllName); 
    return Tools::FindPattern
      (
      (DWORD)hDll, 
      Tools::GetModuleSize(hDll), 
      pattern, 
      mask
      );
  }
}