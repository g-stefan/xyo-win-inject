// Win Inject
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <windows.h>
#include <stdio.h>

#include <XYO/WinInject/Hook.hpp>

#ifdef XYO_APPLICATION_32BIT
#	define mPointer(type_, value_, offset_) (type_)((BYTE *)(value_) + (DWORD)(offset_))
#	define procSize 4
#	define PROCTYPE DWORD
#endif
#ifdef XYO_APPLICATION_64BIT
#	define mPointer(type_, value_, offset_) (type_)((BYTE *)(value_) + (DWORD64)(offset_))
#	define procSize 8
#	define PROCTYPE DWORD64
#endif

namespace XYO::Win::Inject::Hook {

	using namespace XYO;

	void replaceFunction(HMODULE hModule, HookProc **hookList) {
		PIMAGE_DOS_HEADER dosHeader;
		PIMAGE_NT_HEADERS ntHeaders;
		PIMAGE_IMPORT_DESCRIPTOR importDescriptor;
		PIMAGE_THUNK_DATA thunkData;
		DWORD dwOld, dw;
		HookProc **scanList;
		bool found;

		if (hModule != nullptr) {
			dosHeader = (PIMAGE_DOS_HEADER)hModule;
			if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
				ntHeaders = mPointer(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);
				if (ntHeaders->Signature == IMAGE_NT_SIGNATURE) {
					importDescriptor = mPointer(PIMAGE_IMPORT_DESCRIPTOR, dosHeader, ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
					if (importDescriptor > (PIMAGE_IMPORT_DESCRIPTOR)ntHeaders) {
						for (; importDescriptor->Name; ++importDescriptor) {
							for (thunkData = mPointer(PIMAGE_THUNK_DATA, dosHeader, importDescriptor->FirstThunk); thunkData->u1.Function; ++thunkData) {
								for (scanList = hookList; *scanList != nullptr; ++scanList) {
									if ((FARPROC)thunkData->u1.Function == (FARPROC)(*scanList)->originalProc) {
										if (VirtualProtect(&thunkData->u1.Function, procSize,
										                   PAGE_EXECUTE_READWRITE, &dwOld)) {

											thunkData->u1.Function = (PROCTYPE)(*scanList)->newProc;

											VirtualProtect(&thunkData->u1.Function, procSize,
											               dwOld, &dw);
										};
										break;
									};
								};
							};
						};
					};
				};
			};
		};
	};

	BOOL enumImportTable(HMODULE hModule, IEnumImportModuleName iEnum, PVOID userData) {
		PIMAGE_DOS_HEADER dosHeader;
		PIMAGE_NT_HEADERS ntHeaders;
		PIMAGE_IMPORT_DESCRIPTOR importDescriptor;

		if (hModule != nullptr) {
			dosHeader = (PIMAGE_DOS_HEADER)hModule;
			if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
				ntHeaders = mPointer(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);
				if (ntHeaders->Signature == IMAGE_NT_SIGNATURE) {
					importDescriptor = mPointer(PIMAGE_IMPORT_DESCRIPTOR, dosHeader, ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
					if (importDescriptor > (PIMAGE_IMPORT_DESCRIPTOR)ntHeaders) {
						for (; importDescriptor->Name; ++importDescriptor) {
							if ((*iEnum)(userData, mPointer(LPSTR, dosHeader, importDescriptor->Name))) {
							} else {
								return TRUE;
							}
						}
					}
				}
			}
		}
		return TRUE;
	};

	LPSTR getProcOrdinal(HMODULE hModule, LPSTR procName) {
		PIMAGE_DOS_HEADER dosHeader;
		PIMAGE_NT_HEADERS ntHeaders;
		PIMAGE_EXPORT_DIRECTORY exportDescriptor;
		DWORD *procNames;
		DWORD procIndex;

		if (IS_INTRESOURCE(procName)) {
			return procName;
		};

		if (hModule != nullptr) {
			dosHeader = (PIMAGE_DOS_HEADER)hModule;
			if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
				ntHeaders = mPointer(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);
				if (ntHeaders->Signature == IMAGE_NT_SIGNATURE) {
					exportDescriptor = mPointer(PIMAGE_EXPORT_DIRECTORY, dosHeader, ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
					if (exportDescriptor > (PIMAGE_EXPORT_DIRECTORY)ntHeaders) {
						procNames = mPointer(DWORD *, dosHeader, exportDescriptor->AddressOfNames);
						for (procIndex = 0; procIndex < exportDescriptor->NumberOfNames; ++procIndex) {
							if (StringCore::compareIgnoreCaseAscii(mPointer(LPSTR, dosHeader, procNames[procIndex]), procName) == 0) {
								return MAKEINTRESOURCEA(exportDescriptor->Base + ((mPointer(WORD *, dosHeader, exportDescriptor->AddressOfNameOrdinals))[procIndex]));
							};
						};
					};
				};
			};
		};
		return 0;
	};

	LPSTR getProcName(HMODULE hModule, LPSTR procOrdinal) {
		PIMAGE_DOS_HEADER dosHeader;
		PIMAGE_NT_HEADERS ntHeaders;
		PIMAGE_EXPORT_DIRECTORY exportDescriptor;
		DWORD procIndex;

		if (!IS_INTRESOURCE(procOrdinal)) {
			return procOrdinal;
		};

		if (hModule != nullptr) {
			dosHeader = (PIMAGE_DOS_HEADER)hModule;
			if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
				ntHeaders = mPointer(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);
				if (ntHeaders->Signature == IMAGE_NT_SIGNATURE) {
					exportDescriptor = mPointer(PIMAGE_EXPORT_DIRECTORY, dosHeader, ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
					if (exportDescriptor > (PIMAGE_EXPORT_DIRECTORY)ntHeaders) {
						for (procIndex = 0; procIndex < exportDescriptor->NumberOfNames; ++procIndex) {
							if (MAKEINTRESOURCEA(exportDescriptor->Base + ((mPointer(WORD *, dosHeader, exportDescriptor->AddressOfNameOrdinals))[procIndex])) == procOrdinal) {
								return mPointer(LPSTR, dosHeader, (mPointer(DWORD *, dosHeader, exportDescriptor->AddressOfNames))[procIndex]);
							};
						};
					};
				};
			};
		};
		return 0;
	};

	void setOriginalFunction(HookProc &hook, LPSTR moduleName, LPSTR procName, FARPROC newProc) {

		hook.hModule = nullptr;
		hook.newProc = nullptr;
		hook.originalProc = nullptr;
		hook.procName = nullptr;
		hook.procOrdinal = nullptr;

		HMODULE hModule = GetModuleHandle(moduleName);
		if (hModule == nullptr) {
			hModule = LoadLibrary(moduleName);
		};
		if (hModule != nullptr) {
			hook.hModule = hModule;
			hook.newProc = newProc;
			hook.originalProc = GetProcAddress(hModule, procName);
			hook.procName = getProcName(hModule, procName);
			hook.procOrdinal = getProcOrdinal(hModule, procName);
		};
	};

	bool processModule(HMODULE hModule, HookProc **hookList, HMODULE *processedList, size_t &processedListIndex, size_t processedListSize, LPSTR *skipList) {
		PIMAGE_DOS_HEADER dosHeader;
		PIMAGE_NT_HEADERS ntHeaders;
		PIMAGE_IMPORT_DESCRIPTOR importDescriptor;
		size_t index;

		if (hModule == nullptr) {
			return false;
		};

		for (index = 0; index < processedListIndex; ++index) {
			if (hModule == processedList[index]) {
				return false;
			};
		};

		processedList[processedListIndex] = hModule;
		++processedListIndex;

		for (index = 0; skipList[index] != nullptr; ++index) {
			if (hModule == GetModuleHandle(skipList[index])) {
				return false;
			};
		};

		replaceFunction(hModule, hookList);

		dosHeader = (PIMAGE_DOS_HEADER)hModule;
		if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
			ntHeaders = mPointer(PIMAGE_NT_HEADERS, dosHeader, dosHeader->e_lfanew);
			if (ntHeaders->Signature == IMAGE_NT_SIGNATURE) {
				importDescriptor = mPointer(PIMAGE_IMPORT_DESCRIPTOR, dosHeader, ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
				if (importDescriptor > (PIMAGE_IMPORT_DESCRIPTOR)ntHeaders) {
					for (; importDescriptor->Name; ++importDescriptor) {
						processModule(GetModuleHandle(mPointer(LPSTR, dosHeader, importDescriptor->Name)), hookList, processedList, processedListIndex, processedListSize, skipList);
					};
				};
			};
		};

		return true;
	};

	FARPROC getProcAddress(HMODULE hModule, LPCSTR lpProcName, HookProc **hookList) {
		HookProc **scanList;
		if (IS_INTRESOURCE(lpProcName)) {
			for (scanList = hookList; *scanList != nullptr; ++scanList) {
				if ((*scanList)->hModule == hModule) {
					if ((*scanList)->procOrdinal == lpProcName) {
						return (*scanList)->newProc;
					};
				};
			};
			return nullptr;
		};
		for (scanList = hookList; *scanList != nullptr; ++scanList) {
			if ((*scanList)->hModule == hModule) {
				if (StringCore::compareIgnoreCaseAscii((*scanList)->procName, lpProcName) == 0) {
					return (*scanList)->newProc;
				};
			};
		};
		return nullptr;
	};

};
