#include "stdafx.h"

#define Py_BUILD_CORE
#include <Python-2.6.4/Include/Python.h>
#include <cassert>

#include "..\pytest\Tools.h"

PyAPI_FUNC(int) PyRun_SimpleStringFlags(const char * a1, PyCompilerFlags * a2)
{
  typedef int (*tpPyRun_SimpleStringFlags)(const char*, PyCompilerFlags*);

  static tpPyRun_SimpleStringFlags pRun = (tpPyRun_SimpleStringFlags)(Tools::FindPattern(nullptr, 
    "\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04\x85\xC0\x74\x2D", 
    "x????x????xxxxxxx"));
  assert(pRun != nullptr);

  return pRun(a1, a2);
}

PyAPI_FUNC(PyGILState_STATE) PyGILState_Ensure(void)
{
  typedef PyGILState_STATE (*tpPyGILState_Ensure)(void);

  static tpPyGILState_Ensure pGilEnsure = (tpPyGILState_Ensure)(Tools::FindPattern(nullptr, 
    "\xA1\x00\x00\x00\x00\x56\x57\x50\xE8", 
    "x????xxxx"));
  assert(pGilEnsure != nullptr);

  return pGilEnsure();
}

PyAPI_FUNC(void) PyGILState_Release(PyGILState_STATE a1)
{
  typedef void (*tpPyGILState_Release)(PyGILState_STATE);

  static tpPyGILState_Release pGilRelease = (tpPyGILState_Release)(Tools::FindPattern(nullptr, 
    "\xA1\x00\x00\x00\x00\x56\x50\xE8\x00\x00\x00\x00\x8B\xF0\x83\xC4\x04", 
    "x????xxx????xxxxx"));
  assert(pGilRelease != nullptr);

  return pGilRelease(a1);
}
