// Win Inject
// Copyright (c) 2014-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_WININJECT_HOOK_HPP
#define XYO_WININJECT_HOOK_HPP

#ifndef XYO_WININJECT_DEPENDENCY_HPP
#	include <XYO/WinInject/Dependency.hpp>
#endif

namespace XYO::Win::Inject::Hook {

	typedef struct SHookProc {
			HANDLE hModule;
			FARPROC newProc;
			FARPROC originalProc;
			LPSTR procName;
			LPSTR procOrdinal;
	} HookProc;

	typedef BOOL(WINAPI *IEnumImportModuleName)(PVOID userData, LPSTR module);
	XYO_WININJECT_EXPORT BOOL enumImportTable(HMODULE hModule, IEnumImportModuleName iEnum, PVOID userData);
	XYO_WININJECT_EXPORT void replaceFunction(HMODULE hModule, HookProc **hookList);
	XYO_WININJECT_EXPORT LPSTR getProcOrdinal(HMODULE hModule, LPSTR procName);
	XYO_WININJECT_EXPORT LPSTR getProcName(HMODULE hModule, LPSTR procOrdinal);
	XYO_WININJECT_EXPORT void setOriginalFunction(HookProc &hook, LPSTR moduleName, LPSTR procName, FARPROC newProc);
	XYO_WININJECT_EXPORT bool processModule(HMODULE hModule, HookProc **hookList, HMODULE *processedList, size_t &processedListIndex, size_t processedListSize, LPSTR *skipList);
	XYO_WININJECT_EXPORT FARPROC getProcAddress(HMODULE hModule, LPCSTR lpProcName, HookProc **hookList);

};

#endif
