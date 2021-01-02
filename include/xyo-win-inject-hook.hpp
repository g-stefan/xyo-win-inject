//
// XYO Win Inject
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef XYO_WIN_INJECT_HOOK_HPP
#define XYO_WIN_INJECT_HOOK_HPP

#ifndef XYO_WIN_INJECT__DEPENDENCY_HPP
#include "xyo-win-inject--dependency.hpp"
#endif

namespace XYO {
	namespace Win {
		namespace Inject {
			namespace Hook {

				typedef struct SHookProc {
					HANDLE hModule;
					FARPROC newProc;
					FARPROC originalProc;
					LPSTR procName;
					LPSTR procOrdinal;
				} HookProc;

				typedef BOOL (WINAPI *IEnumImportModuleName)(PVOID userData, LPSTR module);
				XYO_WIN_INJECT_EXPORT BOOL enumImportTable(HMODULE hModule, IEnumImportModuleName iEnum, PVOID userData);
				XYO_WIN_INJECT_EXPORT void replaceFunction(HMODULE hModule, HookProc **hookList);
				XYO_WIN_INJECT_EXPORT LPSTR getProcOrdinal(HMODULE hModule, LPSTR procName);
				XYO_WIN_INJECT_EXPORT LPSTR getProcName(HMODULE hModule, LPSTR procOrdinal);
				XYO_WIN_INJECT_EXPORT void setOriginalFunction(HookProc &hook, LPSTR moduleName, LPSTR procName, FARPROC newProc);
				XYO_WIN_INJECT_EXPORT bool processModule(HMODULE hModule, HookProc **hookList, HMODULE *processedList, size_t &processedListIndex, size_t processedListSize, LPSTR *skipList);
				XYO_WIN_INJECT_EXPORT FARPROC getProcAddress(HMODULE hModule, LPCSTR lpProcName, HookProc **hookList);

			};
		};
	};
};

#endif

