/* Author: h0pk1nz
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef PROCTOOLS_H
#define PROCTOOLS_H

#include <Windows.h>

class CProcessTools
{
public:
	static bool		CloseProcess			( HANDLE hProcess );

	static HANDLE	GetProcessById			( DWORD dwProcessId );
	static HANDLE	GetProcessByIdEx		( DWORD dwDiseredAccess, DWORD dwProcessId );
	static HANDLE	GetProcessByName		( const char * pszProcessName );
	static HANDLE	GetProcessByName		( const wchar_t * pszProcessName );
	static HANDLE	GetProcessByNameEx		( DWORD dwDiseredAccess, const char * pszProcessName );
	static HANDLE	GetProcessByNameEx		( DWORD dwDiseredAccess, const wchar_t * pszProcessName );
	static HANDLE	GetProcessByWindow		( HWND hWnd );
	static HANDLE	GetProcessByWindow		( const char * pszWindowName, const char * pszClassName );
	static HANDLE	GetProcessByWindow		( const wchar_t * pszWindowName, const wchar_t * pszClassName );
	static HANDLE	GetProcessByWindowEx	( DWORD dwDiseredAccess, HWND hWnd );
	static HANDLE	GetProcessByWindowEx	( DWORD dwDiseredAccess, const char * pszWindowName, const char * pszClassName );
	static HANDLE	GetProcessByWindowEx	( DWORD dwDiseredAccess, const wchar_t * pszWindowName, const wchar_t * pszClassName );

	static bool		ExternalFindPtrnCmp		( HANDLE hProcess, DWORD dwAddress, BYTE * pMask, char * pszMask, DWORD dwBlockSize );
	static DWORD	ExternalFindPattern		( HANDLE hProcess, DWORD dwStartAddress, BYTE * pMask, char * pszMask );

	static HMODULE	ExternalLoadLibrary		( DWORD dwProcId, const char * pszModulePath );
	static HMODULE	ExternalLoadLibrary		( HANDLE hProcess, const char * pszModulePath );

	static FARPROC	ExternalGetProcAddress	( DWORD dwProcId, const char * pszModule, const char * pszFunction );
	static FARPROC	ExternalGetProcAddress	( HANDLE hProcess, const char * pszModule, const char * pszFunction );

	static HMODULE	ExternalGetModuleHandle	( DWORD dwProcId, const char * pszModuleName );
	static HMODULE	ExternalGetModuleHandle	( HANDLE hProcess, const char * pszModuleName );
};

typedef CProcessTools ProcTools;

#define CLOSE_PROCESS( handle ) ProcTools::CloseProcess( handle )

#endif // PROCTOOLS_H