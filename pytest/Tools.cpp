#include "stdafx.h"
#include "Tools.h"
#include <Tlhelp32.h>

namespace Tools
{
  DWORD GetModulePath( HINSTANCE hInst, char* pszBuffer, DWORD dwSize )
  {
    DWORD dwLength = GetModuleFileNameA( hInst, pszBuffer, dwSize );
    if( dwLength )
    {
      while( dwLength && pszBuffer[ dwLength ] != '\\' )
      {
        dwLength--;
      }

      if( dwLength )
        pszBuffer[ dwLength + 1 ] = '\0';
    }
    return dwLength;
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