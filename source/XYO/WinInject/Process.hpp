// Win Inject
// Copyright (c) 2014-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_WININJECT_PROCESS_HPP
#define XYO_WININJECT_PROCESS_HPP

#ifndef XYO_WININJECT_DEPENDENCY_HPP
#	include <XYO/WinInject/Dependency.hpp>
#endif

namespace XYO::Win::Inject::Process {

	XYO_WININJECT_EXPORT BOOL injectDll(char *cmdLine, const char *dllFile);
	XYO_WININJECT_EXPORT BOOL injectDllAndWait(char *cmdLine, const char *dllFile);
	XYO_WININJECT_EXPORT BOOL injectDllDirect(HANDLE hProcess, HANDLE hThread, const char *dllFile);
	XYO_WININJECT_EXPORT BOOL createProcessA(
	    LPCTSTR lpApplicationName,
	    LPTSTR lpCommandLine,
	    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	    BOOL bInheritHandles,
	    DWORD dwCreationFlags,
	    LPVOID lpEnvironment,
	    LPCTSTR lpCurrentDirectory,
	    LPSTARTUPINFOA lpStartupInfo,
	    LPPROCESS_INFORMATION lpProcessInformation,
	    const char *dllFile);
	XYO_WININJECT_EXPORT BOOL createProcessW(
	    LPCWSTR lpApplicationName,
	    LPWSTR lpCommandLine,
	    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	    BOOL bInheritHandles,
	    DWORD dwCreationFlags,
	    LPVOID lpEnvironment,
	    LPCWSTR lpCurrentDirectory,
	    LPSTARTUPINFOW lpStartupInfo,
	    LPPROCESS_INFORMATION lpProcessInformation,
	    const char *dllFile);
	XYO_WININJECT_EXPORT BOOL createProcessAsUserA(
	    HANDLE hToken,
	    LPCTSTR lpApplicationName,
	    LPTSTR lpCommandLine,
	    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	    BOOL bInheritHandles,
	    DWORD dwCreationFlags,
	    LPVOID lpEnvironment,
	    LPCTSTR lpCurrentDirectory,
	    LPSTARTUPINFOA lpStartupInfo,
	    LPPROCESS_INFORMATION lpProcessInformation,
	    const char *dllFile);
	XYO_WININJECT_EXPORT BOOL createProcessAsUserW(
	    HANDLE hToken,
	    LPCWSTR lpApplicationName,
	    LPWSTR lpCommandLine,
	    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	    BOOL bInheritHandles,
	    DWORD dwCreationFlags,
	    LPVOID lpEnvironment,
	    LPCWSTR lpCurrentDirectory,
	    LPSTARTUPINFOW lpStartupInfo,
	    LPPROCESS_INFORMATION lpProcessInformation,
	    const char *dllFile);

};

#endif
