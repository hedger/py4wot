#include "stdafx.h"
#include "SelfInject.h"
#include <ProcTools.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

bool SelfInject(const char* targetProcess)
{
  char ModulePath[MAX_PATH];
  GetModuleFileNameA((HMODULE)&__ImageBase, ModulePath, sizeof(ModulePath));
  return (CProcessTools::ExternalLoadLibrary(
    CProcessTools::GetProcessByName(targetProcess),
    ModulePath) != INVALID_HANDLE_VALUE);
};

bool IsRunFromRunDll()
{
  WCHAR ModulePath[MAX_PATH];
  GetModuleFileName(GetModuleHandle(NULL), ModulePath, sizeof(ModulePath));
  return (wcsstr(ModulePath, L"dll32") != NULL);
};

extern "C" __declspec (dllexport) 
  void __cdecl Inject(HWND hwnd, HINSTANCE hinst, LPTSTR lpCmdLine, int nCmdShow)
{
  if (!SelfInject("WorldOfWarplanes.exe"))
    MessageBox(0, L"Failed to inject!", L"...", 32);
};

void UnloadDLL(HMODULE hModule)
{
  LPVOID ExitThreadAddress = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "ExitThread");
  _asm
  {
    push hModule
    push ExitThreadAddress
    jmp dword ptr [FreeLibrary]
  }
}
