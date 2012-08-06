#include "stdafx.h"

#include "tools.h"
#include "console.h"
#include "SelfInject.h"
#include <iostream>
#include <string>

#define Py_BUILD_CORE
#include <Python-2.6.4/Include/Python.h>

// import sys; sys.stdout = open('CONOUT$', 'wt'); print "lol"
DWORD WINAPI MainThread(LPVOID handle)
{
  Sleep(1000);
  HMODULE self = static_cast<HMODULE>(handle);
  Console::Open();
  printf("py4wot 0.1 loaded!\n\n");

  //std::string cmd = "print(\"Hello from Python!\")";
  PyRun_SimpleStringFlags("import sys; sys.stdout = open(\"CONOUT$\", \"wt\")", 0);
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
        printf("... ");
      }
    }
    while (!cmd.empty());

    PyGILState_STATE gstate = PyGILState_Ensure();
    //printf("Executing >%s<\n", batch.c_str());
    //printf("Result = %d\n", PyRun_SimpleStringFlags(batch.c_str(), 0));
    int ret = PyRun_SimpleStringFlags(batch.c_str(), 0);
    if (ret == -1)
      printf("Error.\n");
    PyRun_SimpleStringFlags("sys.stdout.flush()", 0);
    PyGILState_Release(gstate);
    fflush(stdout);
    fflush(stdin);
    fflush(stderr);
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

