#pragma once

#include <Windows.h>

bool IsRunFromRunDll();

extern "C" __declspec (dllexport) 
  void __cdecl Inject(HWND hwnd, HINSTANCE hinst, LPTSTR lpCmdLine, int nCmdShow);