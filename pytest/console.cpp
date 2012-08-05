#include "stdafx.h"
#include "console.h"

#include <cstdio>
#include <io.h>
#include <fcntl.h>

namespace Console
{
  HANDLE hOut=NULL;
  HANDLE StdErr=NULL;
  HANDLE StdIn=NULL;
  HANDLE StdOut=NULL;

  BOOL WINAPI ConsoleHandlerRoutine(DWORD  /*dwCtrlType*/)
  {
    return(1);
  }

  bool Open() 
  {
    int          hCrt;
    FILE *       hf;

    if (!AllocConsole())
      return false;

    SetConsoleTitle(L"Command Prompt");
    SetConsoleCtrlHandler(ConsoleHandlerRoutine, 1);

    StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    hCrt = _open_osfhandle(( intptr_t )StdOut, _O_TEXT);
    hf = _fdopen(hCrt, "w");
    *stdout = *hf;
    setvbuf(stdout, NULL, _IONBF, 0);
    //SetStdHandle(STD_OUTPUT_HANDLE, hf);

    StdErr = GetStdHandle(STD_ERROR_HANDLE);
    hCrt = _open_osfhandle(( intptr_t )StdErr, _O_TEXT);
    hf = _fdopen(hCrt, "w");
    *stderr = *hf;
    setvbuf(stdout, NULL, _IONBF, 0);
    //SetStdHandle(STD_ERROR_HANDLE, hf);

    StdIn = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((intptr_t)StdIn, _O_TEXT);
    hf = _fdopen(hCrt, "r");
    *stdin = *hf;
    setvbuf(stdin, NULL, _IONBF, 0); 
    //SetStdHandle(STD_INPUT_HANDLE, hf);

    return true;
  }

  bool Close() 
  {
    SetStdHandle(STD_INPUT_HANDLE, StdIn);
    SetStdHandle(STD_OUTPUT_HANDLE, StdOut);
    SetStdHandle(STD_ERROR_HANDLE, StdErr);
    return FreeConsole();
  }

}