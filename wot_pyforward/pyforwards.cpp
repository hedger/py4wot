#include "stdafx.h"

#define Py_BUILD_CORE
#include <Python-2.6.4/Include/Python.h>
#include <cassert>

#include "..\pytest\Tools.h"

PyAPI_FUNC(int) PyRun_SimpleStringFlags(const char * a1, PyCompilerFlags * a2)
{
  typedef int (*tpPyRun_SimpleStringFlags)(const char*, PyCompilerFlags*);

  static tpPyRun_SimpleStringFlags pRun = (tpPyRun_SimpleStringFlags)(Tools::FindPattern("worldoftanks.exe", 
    "\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04\x85\xC0\x74\x28\x50", 
    "x????x????xxxxxxxx"));
  assert(pRun != nullptr);
  
  return pRun(a1, a2);
}

PyAPI_FUNC(PyGILState_STATE) PyGILState_Ensure(void)
{
  typedef PyGILState_STATE (*tpPyGILState_Ensure)(void);

  static tpPyGILState_Ensure pGilEnsure = (tpPyGILState_Ensure)(Tools::FindPattern("worldoftanks.exe", 
    "\x56\x57\xE8\x00\x00\x00\x00\x8B\xF0\x85\xF6\x75\x2B", 
    "xxx????xxxxxx"));
  assert(pGilEnsure != nullptr);

  return pGilEnsure();
}

PyAPI_FUNC(void) PyGILState_Release(PyGILState_STATE a1)
{
  typedef void (*tpPyGILState_Release)(PyGILState_STATE);

  static tpPyGILState_Release pGilRelease = (tpPyGILState_Release)(Tools::FindPattern("worldoftanks.exe", 
    "\x53\x56\xE8\x00\x00\x00\x00\x8B\xF0\x85\xF6\x75\x0A\xBB\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xCE", 
    "xxx????xxxxxxx????x????xx"));
  assert(pGilRelease != nullptr);

  return pGilRelease(a1);
}
