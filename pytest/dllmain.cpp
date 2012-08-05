#include "stdafx.h"

#include "tools.h"
#include "console.h"
#include "SelfInject.h"
#include <iostream>
#include <string>

#define Py_BUILD_CORE
#include <Python-2.6.4/Include/Python.h>
#pragma comment(lib, "../Debug/wot_pyforward.lib")  // FIXME

// import sys; sys.stdout = open('CONOUT$', 'wt'); print "lol"
DWORD WINAPI MainThread(LPVOID handle)
{
  Sleep(1000);
  HMODULE self = static_cast<HMODULE>(handle);
  Console::Open();
  printf("Hello world!\n");

  //std::string cmd = "print(\"Hello from Python!\")";
  std::string cmd = "import sys; sys.stdout = open(\"CONOUT$\", \"wt\")";
  while (!cmd.empty())
  {
    //printf("Executing >%s<\n", cmd.c_str())
    PyGILState_STATE gstate = PyGILState_Ensure();
    printf("Result = %d\n", PyRun_SimpleStringFlags(cmd.c_str(), 0));
    PyRun_SimpleStringFlags("sys.stdout.flush()", 0);
    PyGILState_Release(gstate);
    fflush(stdout);
    fflush(stdin);
    fflush(stderr);
    std::getline(std::cin, cmd);
  }

  printf("Unloading in 2s...\n");
  Sleep(2000);
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

