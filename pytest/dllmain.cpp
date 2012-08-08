#include "stdafx.h"

#include "tools.h"
#include "console.h"
#include "SelfInject.h"
#include <iostream>
#include <string>
#include <algorithm>

#define Py_BUILD_CORE
#include <Python-2.6.4/Include/Python.h>

bool RunPython(const std::string& code)
{
  PyGILState_STATE gstate = PyGILState_Ensure();
  int ret = PyRun_SimpleStringFlags(code.c_str(), 0);
  PyRun_SimpleStringFlags("sys.stdout.flush()", 0);
  PyGILState_Release(gstate);
  return (ret != -1);
}

bool active = true;
DWORD WINAPI ConFlusher(LPVOID)
{
  while(active)
  {
    RunPython("");
    Sleep(50);
  }
  return 0;  
}

DWORD WINAPI MainThread(LPVOID handle)
{
  Sleep(1000);
  HMODULE self = static_cast<HMODULE>(handle);
  Console::Open();
  printf("py4wot 0.1 loaded!\n"
    "Type unload when done\n\n");

  char modPath[255];
  size_t len = Tools::GetModulePath(static_cast<HINSTANCE>(handle), modPath, 255);
  std::replace(modPath, modPath+len+1, '\\', '/');
  std::string scriptPath(modPath);
  scriptPath.append("scripts/");
  printf("Script path: %s\n", scriptPath.c_str());

  RunPython("logger = open(\"CONOUT$\", \"wt\"); import sys, os; "
    "old_out = sys.stdout; old_err = sys.stderr;"
    "sys.stdout = logger; sys.stderr = logger");
  RunPython(std::string("sys.path.append(\"") + scriptPath + "\")");
  //RunPython(std::string("os.chdir(\"") + scriptPath + "\")");
  RunPython(std::string("def run(f):\n  execfile(\"%s/%s\" % (\"") + scriptPath + "\", f), globals(), globals())");
  RunPython("run(\"autoexec.py\")");

  CreateThread(NULL, NULL, &ConFlusher, NULL, NULL, NULL);
  std::string cmd, batch;
  while (batch.compare("unload"))
  {
    batch = "", cmd = "";
    printf(">>> ");
    do
    {
      std::getline(std::cin, cmd);
      if (!batch.empty() && !cmd.empty())
        batch.append("\n");
      if (!cmd.empty())
      {
        batch.append(cmd);
        //if (GetAsyncKeyState(VK_CONTROL) & 0x8000 == 0)
        //  break;
        printf("... ");
      }
    }
    while (!cmd.empty());

    if (!RunPython(batch))
      printf("Error.\n");
  }
  RunPython("logger.close(); sys.stdout = old_out; sys.stderr = old_err");
  RunPython("sys.path.pop()");
  printf("Unloading..."); // in 2s...\n");
  active = false;
  Sleep(500);
  Console::Close();
  Tools::UnloadDLL(self);
  return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
    if (!IsRunFromRunDll())
      CreateThread(NULL, NULL, &MainThread, hModule, NULL, NULL);
    break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

