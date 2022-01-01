//
// XYO Win Inject
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef XYO_WIN_INJECT_PROCESS_HPP
#define XYO_WIN_INJECT_PROCESS_HPP

#ifndef XYO_WIN_INJECT__DEPENDENCY_HPP
#include "xyo-win-inject--dependency.hpp"
#endif

namespace XYO {
	namespace Win {
		namespace Inject {
			namespace Process {

				XYO_WIN_INJECT_EXPORT BOOL injectDll(char *cmdLine, const char *dllFile);
				XYO_WIN_INJECT_EXPORT BOOL injectDllAndWait(char *cmdLine, const char *dllFile);
				XYO_WIN_INJECT_EXPORT BOOL injectDllDirect(HANDLE hProcess, HANDLE hThread, const char *dllFile);
				XYO_WIN_INJECT_EXPORT BOOL createProcessA(
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
				XYO_WIN_INJECT_EXPORT BOOL createProcessW(
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
				XYO_WIN_INJECT_EXPORT BOOL createProcessAsUserA(
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
				XYO_WIN_INJECT_EXPORT BOOL createProcessAsUserW(
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
		};
	};
};

#endif

