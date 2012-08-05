/* Author: h0pk1nz
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "ProcTools.h"

#ifdef UNICODE
#undef UNICODE
#endif

#include <TlHelp32.h>

bool CProcessTools::CloseProcess( HANDLE hProcess )
{
	if ( !hProcess ) return false;

	return CloseHandle( hProcess ) ? true : false;
}

HANDLE CProcessTools::GetProcessById( DWORD dwProcessId )
{
	return GetProcessByIdEx( PROCESS_ALL_ACCESS, dwProcessId );
}

HANDLE CProcessTools::GetProcessByIdEx( DWORD dwDiseredAccess, DWORD dwProcessId )
{
	return OpenProcess( dwDiseredAccess, FALSE, dwProcessId );
}

HANDLE CProcessTools::GetProcessByName( const char * pszProcessName )
{
	return GetProcessByNameEx( PROCESS_ALL_ACCESS, pszProcessName );
}

HANDLE CProcessTools::GetProcessByName( const wchar_t * pszProcessName )
{
	return GetProcessByNameEx( PROCESS_ALL_ACCESS, pszProcessName );
}

HANDLE CProcessTools::GetProcessByNameEx( DWORD dwDiseredAccess, const char * pszProcessName )
{
	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof( PROCESSENTRY32 );

	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if ( hSnapshot == INVALID_HANDLE_VALUE ) return NULL;

	if ( !Process32First( hSnapshot, &ProcessEntry ) ) return NULL;

	do 
	{
		if ( !strcmpi( pszProcessName, ProcessEntry.szExeFile ) )
		{
			CloseHandle( hSnapshot );

			return GetProcessByIdEx( dwDiseredAccess, ProcessEntry.th32ProcessID );
		}
	} 
	while ( Process32Next( hSnapshot, &ProcessEntry ) );

	CloseHandle( hSnapshot );

	return NULL;
}

HANDLE CProcessTools::GetProcessByNameEx( DWORD dwDiseredAccess, const wchar_t * pszProcessName )
{
	char szProcessNameA[255];
	ZeroMemory( szProcessNameA, sizeof( szProcessNameA ) );

	wcstombs( szProcessNameA, pszProcessName, wcslen( pszProcessName ) );

	return GetProcessByNameEx( dwDiseredAccess, szProcessNameA );
}

HANDLE CProcessTools::GetProcessByWindow( HWND hWnd )
{
	return GetProcessByWindowEx( PROCESS_ALL_ACCESS, hWnd );
}

HANDLE CProcessTools::GetProcessByWindow( const char * pszWindowName, const char * pszClassName )
{
	return GetProcessByWindowEx( PROCESS_ALL_ACCESS, pszWindowName, pszClassName );
}

HANDLE CProcessTools::GetProcessByWindow( const wchar_t * pszWindowName, const wchar_t * pszClassName )
{
	return GetProcessByWindowEx( PROCESS_ALL_ACCESS, pszWindowName, pszClassName );
}

HANDLE CProcessTools::GetProcessByWindowEx( DWORD dwDiseredAccess, HWND hWnd )
{
	if ( !hWnd ) return NULL;

	DWORD dwProcessId = 0;

	GetWindowThreadProcessId( hWnd, &dwProcessId );

	if ( !dwProcessId ) return NULL;

	return GetProcessByIdEx( dwDiseredAccess, dwProcessId );
}

HANDLE CProcessTools::GetProcessByWindowEx( DWORD dwDiseredAccess, const char * pszWindowName, const char * pszClassName )
{
	HWND hWindow = FindWindowA( pszClassName, pszWindowName );

	return GetProcessByWindow( hWindow );
}

HANDLE CProcessTools::GetProcessByWindowEx( DWORD dwDiseredAccess, const wchar_t * pszWindowName, const wchar_t * pszClassName )
{
	HWND hWindow = FindWindowW( pszClassName, pszWindowName );

	return GetProcessByWindow( hWindow );
}

bool CProcessTools::ExternalFindPtrnCmp( HANDLE hProcess, DWORD dwAddress, BYTE * pMask, char * pszMask, DWORD dwBlockSize )
{
	BYTE bBuffer[255] = { 0 };

	if ( !ReadProcessMemory( hProcess, ( LPCVOID )( dwAddress ), bBuffer, dwBlockSize, NULL ) )
		return false;

	for ( DWORD I = 0; I < dwBlockSize; I++ )
	{
		if ( ( *pszMask == 'x' ) && ( bBuffer[I] != pMask[I] ) )
			return false;
	}

	return true;
}

DWORD CProcessTools::ExternalFindPattern( HANDLE hProcess, DWORD dwStartAddress, BYTE * pMask, char * pszMask )
{
	DWORD dwBlockSize = strlen( pszMask );

	for ( DWORD dwCurrent = dwStartAddress; dwCurrent < 0x7FFFFFFF; dwCurrent += dwBlockSize )
	{
		if ( ExternalFindPtrnCmp( hProcess, dwCurrent, pMask, pszMask, dwBlockSize ) )
			return dwCurrent;
	}

	return 0;
}

HMODULE	CProcessTools::ExternalLoadLibrary( DWORD dwProcId, const char * pszModulePath )
{
	if ( !dwProcId ) return NULL;

	HANDLE hProcess = GetProcessById( dwProcId );

	if ( !hProcess ) return NULL;

	HMODULE hResult = ExternalLoadLibrary( hProcess, pszModulePath );

	CloseHandle( hProcess );

	return hResult;
}

HMODULE	CProcessTools::ExternalLoadLibrary( HANDLE hProcess, const char * pszModulePath )
{
	if ( !hProcess ) return NULL;

	FARPROC pfnLoadLibrary = GetProcAddress( GetModuleHandleA( "kernel32.dll" ), "LoadLibraryA" );

	if ( !pfnLoadLibrary ) return NULL;

	LPVOID pModulePathString = VirtualAllocEx( hProcess, NULL, strlen( pszModulePath ), 
		MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );

	if ( !pModulePathString ) return NULL;

	if ( !WriteProcessMemory( hProcess, pModulePathString, pszModulePath, 
		strlen( pszModulePath ), NULL ) ) return NULL;

	HANDLE hRemoteThread = CreateRemoteThread( hProcess, NULL, 0, 
		( LPTHREAD_START_ROUTINE )( pfnLoadLibrary ), pModulePathString, 0, NULL );

	if ( !hRemoteThread ) return NULL;
	
	HMODULE hResult = NULL;

	DWORD dwThreadResult = 0;

	while ( GetExitCodeThread( hRemoteThread, &dwThreadResult ) )
	{
		if ( dwThreadResult != STILL_ACTIVE )
		{
			hResult = ( HMODULE )( dwThreadResult );

			break;
		}
	}

	CloseHandle( hRemoteThread );

	return hResult;
}

FARPROC CProcessTools::ExternalGetProcAddress( DWORD dwProcId, const char * pszModule, const char * pszFunction )
{
	if ( !dwProcId ) return NULL;

	HANDLE hProcess = GetProcessById( dwProcId );

	if ( !hProcess ) return NULL;

	return ExternalGetProcAddress( hProcess, pszModule, pszFunction );
}

FARPROC CProcessTools::ExternalGetProcAddress( HANDLE hProcess, const char * pszModule, const char * pszFunction )
{
	HMODULE hModuleEx = ExternalGetModuleHandle( hProcess, pszModule );

	if ( !hModuleEx ) return NULL;

	HMODULE hModuleIn = GetModuleHandleA( pszModule );

	if ( !hModuleIn )
	{
		// TODO: try load with full path
		hModuleIn = LoadLibraryExA( pszModule, NULL, DONT_RESOLVE_DLL_REFERENCES );

		if ( !hModuleIn ) return NULL;
	}

	DWORD dwDelta = ( DWORD )( hModuleEx - hModuleIn );

	return ( FARPROC )( ( DWORD )( GetProcAddress( hModuleIn, pszFunction ) ) + dwDelta );
}

HMODULE	CProcessTools::ExternalGetModuleHandle( DWORD dwProcId, const char * pszModuleName )
{
	if ( !dwProcId ) return NULL;

	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwProcId );

	if ( hSnapshot == INVALID_HANDLE_VALUE ) return NULL;

	MODULEENTRY32 ModuleEntry;
	ModuleEntry.dwSize = sizeof( MODULEENTRY32 );

	if ( !Module32First( hSnapshot, &ModuleEntry ) ) return NULL;

	HMODULE hResult = NULL;

	do 
	{
		if ( !strcmpi( pszModuleName, ModuleEntry.szModule ) )
		{
			hResult = ModuleEntry.hModule;

			break;
		}
	}
	while ( Module32Next( hSnapshot, &ModuleEntry ) );

	CloseHandle( hSnapshot );

	return hResult;
}

HMODULE	CProcessTools::ExternalGetModuleHandle( HANDLE hProcess, const char * pszModuleName )
{
	if ( !hProcess ) return NULL;

	DWORD dwProcId = GetProcessId( hProcess );

	return ExternalGetModuleHandle( dwProcId, pszModuleName );
}