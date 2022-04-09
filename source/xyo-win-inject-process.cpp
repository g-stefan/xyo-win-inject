//
// XYO Win Inject
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <windows.h>
#include <stdio.h>
#include "xyo-win-inject-process.hpp"

namespace XYO {
	namespace Win {
		namespace Inject {
			namespace Process {

				BOOL injectDll(char *cmdLine, const char *dllFile) {
					STARTUPINFO SInfo;
					PROCESS_INFORMATION PInfo;
					ZeroMemory(&SInfo, sizeof(SInfo));
					return createProcessA(0, cmdLine, 0, 0, FALSE, 0, 0, 0, &SInfo, &PInfo, dllFile);
				};

				BOOL injectDllAndWait(char *cmdLine, const char *dllFile) {
					STARTUPINFO SInfo;
					PROCESS_INFORMATION PInfo;
					BOOL retVal;
					ZeroMemory(&SInfo, sizeof(SInfo));
					retVal = createProcessA(0, cmdLine, 0, 0, FALSE, 0, 0, 0, &SInfo, &PInfo, dllFile);
					if (retVal) {
						WaitForSingleObject(PInfo.hThread, INFINITE);
					};
					return retVal;
				};

				BOOL createProcessA(
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
				    const char *dllFile) {

					BOOL retVal;
					BOOL isSuspended;

					isSuspended = ((dwCreationFlags & CREATE_SUSPENDED) == CREATE_SUSPENDED);

					retVal = (CreateProcessA(
					              lpApplicationName,
					              lpCommandLine,
					              lpProcessAttributes,
					              lpThreadAttributes,
					              bInheritHandles,
					              dwCreationFlags | CREATE_SUSPENDED,
					              lpEnvironment,
					              lpCurrentDirectory,
					              lpStartupInfo,
					              lpProcessInformation) == TRUE);

					if (retVal) {
						if (injectDllDirect(lpProcessInformation->hProcess, lpProcessInformation->hThread, dllFile)) {
							if (isSuspended) {
							} else {
								ResumeThread(lpProcessInformation->hThread);
							};
						} else {
							TerminateProcess(lpProcessInformation->hProcess, 0);
							retVal = FALSE;
						};
					};
					return retVal;
				};

				BOOL createProcessW(
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
				    const char *dllFile) {

					BOOL retVal;
					BOOL isSuspended;
					isSuspended = ((dwCreationFlags & CREATE_SUSPENDED) == CREATE_SUSPENDED);

					retVal = (CreateProcessW(
					              lpApplicationName,
					              lpCommandLine,
					              lpProcessAttributes,
					              lpThreadAttributes,
					              bInheritHandles,
					              dwCreationFlags | CREATE_SUSPENDED,
					              lpEnvironment,
					              lpCurrentDirectory,
					              lpStartupInfo,
					              lpProcessInformation) == TRUE);

					if (retVal) {
						if (injectDllDirect(lpProcessInformation->hProcess, lpProcessInformation->hThread, dllFile)) {
							if (isSuspended) {
							} else {
								ResumeThread(lpProcessInformation->hThread);
							};
						} else {
							TerminateProcess(lpProcessInformation->hProcess, 0);
							retVal = FALSE;
						};
					};
					return retVal;
				};

				BOOL createProcessAsUserA(
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
				    const char *dllFile) {

					BOOL retVal;
					BOOL isSuspended;

					isSuspended = ((dwCreationFlags & CREATE_SUSPENDED) == CREATE_SUSPENDED);

					retVal = (CreateProcessAsUserA(
					              hToken,
					              lpApplicationName,
					              lpCommandLine,
					              lpProcessAttributes,
					              lpThreadAttributes,
					              bInheritHandles,
					              dwCreationFlags | CREATE_SUSPENDED,
					              lpEnvironment,
					              lpCurrentDirectory,
					              lpStartupInfo,
					              lpProcessInformation) == TRUE);

					if (retVal) {
						if (injectDllDirect(lpProcessInformation->hProcess, lpProcessInformation->hThread, dllFile)) {
							if (isSuspended) {
							} else {
								ResumeThread(lpProcessInformation->hThread);
							};
						} else {
							TerminateProcess(lpProcessInformation->hProcess, 0);
							retVal = FALSE;
						};
					};
					return retVal;
				};

				BOOL createProcessAsUserW(
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
				    const char *dllFile) {

					BOOL retVal;
					BOOL isSuspended;
					isSuspended = ((dwCreationFlags & CREATE_SUSPENDED) == CREATE_SUSPENDED);

					retVal = (CreateProcessAsUserW(
					              hToken,
					              lpApplicationName,
					              lpCommandLine,
					              lpProcessAttributes,
					              lpThreadAttributes,
					              bInheritHandles,
					              dwCreationFlags | CREATE_SUSPENDED,
					              lpEnvironment,
					              lpCurrentDirectory,
					              lpStartupInfo,
					              lpProcessInformation) == TRUE);

					if (retVal) {
						if (injectDllDirect(lpProcessInformation->hProcess, lpProcessInformation->hThread, dllFile)) {
							if (isSuspended) {
							} else {
								ResumeThread(lpProcessInformation->hThread);
							};
						} else {
							TerminateProcess(lpProcessInformation->hProcess, 0);
							retVal = FALSE;
						};
					};
					return retVal;
				};

#define idx_ptr_ptr_loadlibrary 64
#define idx_ptr_thread_entrypoint (idx_ptr_ptr_loadlibrary + 8)
#define idx_ptr_str (idx_ptr_thread_entrypoint + 8)

#ifdef XYO_APPLICATION_32BIT
				BOOL injectDllDirect(HANDLE hProcess, HANDLE hThread, const char *dllFile) {
					CONTEXT Context;
					BYTE *PocessMem;
					DWORD ThreadEntryPoint;
					DWORD ptr_LoadLibraryA;
					DWORD ptr_ptr_LoadLibraryA;
					DWORD ptr_Str;
					DWORD ptr_ThreadEntryPoint;
					BYTE ThisMem[4096];
					INT k_;
					INT ip;

					PocessMem = (BYTE *)VirtualAllocEx(hProcess, 0, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
					if (PocessMem == NULL) {
						return FALSE;
					};

					Context.ContextFlags = CONTEXT_CONTROL;
					if (!GetThreadContext(hThread, &Context)) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};

					ThreadEntryPoint = Context.Eip;

					FillMemory(ThisMem, 4096, 0xC3); // ret

					HMODULE hKernel32 = GetModuleHandle("KERNEL32.DLL");
					if (hKernel32 == NULL) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};
					ptr_LoadLibraryA = (DWORD64)GetProcAddress(hKernel32, "LoadLibraryA");
					ptr_Str = (DWORD)&PocessMem[idx_ptr_str];
					ptr_ptr_LoadLibraryA = (DWORD)&PocessMem[idx_ptr_ptr_loadlibrary];
					ptr_ThreadEntryPoint = (DWORD)&PocessMem[idx_ptr_thread_entrypoint];

					MoveMemory(&ThisMem[idx_ptr_ptr_loadlibrary], &ptr_LoadLibraryA, 4);
					MoveMemory(&ThisMem[idx_ptr_thread_entrypoint], &ThreadEntryPoint, 4);
					for (k_ = 0; dllFile[k_] != 0; ++k_) {
						ThisMem[idx_ptr_str + k_] = dllFile[k_];
					};
					ThisMem[idx_ptr_str + k_] = 0;

					ip = 0;

					// pushad
					ThisMem[ip] = 0x60;
					ip += 1;

					// push str ptr
					ThisMem[ip] = 0x68;
					ip += 1;
					MoveMemory(&ThisMem[ip], &ptr_Str, 4);
					ip += 4;

					// call dword[LoadLibray]
					ThisMem[ip] = 0xFF;
					ip += 1;
					ThisMem[ip] = 0x15;
					ip += 1;
					MoveMemory(&ThisMem[ip], &ptr_ptr_LoadLibraryA, 4);
					ip += 4;

					// popad
					ThisMem[ip] = 0x61;
					ip += 1;

					// jmp dword[EntryPtr]
					ThisMem[ip] = 0xFF;
					ip += 1;
					ThisMem[ip] = 0x25;
					ip += 1;
					MoveMemory(&ThisMem[ip], &ptr_ThreadEntryPoint, 4);
					ip += 4;

					if (!WriteProcessMemory(hProcess, PocessMem, ThisMem, 4096, NULL)) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};

					FlushInstructionCache(hProcess, PocessMem, 4096);

					Context.ContextFlags = CONTEXT_CONTROL;
					if (!GetThreadContext(hThread, &Context)) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};

					Context.Eip = (DWORD)PocessMem;

					if (!SetThreadContext(hThread, &Context)) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};

					return TRUE;
				};
#endif

#ifdef XYO_APPLICATION_64BIT
				BOOL injectDllDirect(HANDLE hProcess, HANDLE hThread, const char *dllFile) {
					CONTEXT Context;
					BYTE *PocessMem;

					DWORD64 ThreadEntryPoint;
					DWORD64 ptr_LoadLibraryA;
					DWORD64 ptr_ptr_LoadLibraryA;
					DWORD64 ptr_Str;
					DWORD64 ptr_ThreadEntryPoint;

					BYTE ThisMem[4096];
					INT k_;
					INT ip;

					PocessMem = (BYTE *)VirtualAllocEx(hProcess, 0, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
					if (PocessMem == NULL) {
						return FALSE;
					};

					Context.ContextFlags = CONTEXT_CONTROL;
					if (!GetThreadContext(hThread, &Context)) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};

					ThreadEntryPoint = Context.Rip;

					FillMemory(ThisMem, 4096, 0xC3); // ret

					HMODULE hKernel32 = GetModuleHandle("KERNEL32.DLL");
					if (hKernel32 == NULL) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};
					ptr_LoadLibraryA = (DWORD64)GetProcAddress(hKernel32, "LoadLibraryA");
					ptr_Str = (DWORD64)&PocessMem[idx_ptr_str];
					ptr_ptr_LoadLibraryA = (DWORD64)&PocessMem[idx_ptr_ptr_loadlibrary];
					ptr_ThreadEntryPoint = (DWORD64)&PocessMem[idx_ptr_thread_entrypoint];

					MoveMemory(&ThisMem[idx_ptr_ptr_loadlibrary], &ptr_LoadLibraryA, 8);
					MoveMemory(&ThisMem[idx_ptr_thread_entrypoint], &ThreadEntryPoint, 8);

					for (k_ = 0; dllFile[k_] != 0; ++k_) {
						ThisMem[idx_ptr_str + k_] = dllFile[k_];
					};
					ThisMem[idx_ptr_str + k_] = 0;

					ip = 0;

					// 51 push rcx
					ThisMem[ip] = 0x51;
					ip += 1;

					// 48 83 EC 28        sub         rsp,28h  - using 50h for safe execution
					ThisMem[ip] = 0x48;
					ip += 1;
					ThisMem[ip] = 0x83;
					ip += 1;
					ThisMem[ip] = 0xEC;
					ip += 1;
					ThisMem[ip] = 0x50;
					ip += 1;

					// mov rcx, adr..
					ThisMem[ip] = 0x48;
					ip += 1;
					ThisMem[ip] = 0xB9;
					ip += 1;
					MoveMemory(&ThisMem[ip], &ptr_Str, 8);
					ip += 8;

					// mov rax, ptr load library
					ThisMem[ip] = 0x48;
					ip += 1;
					ThisMem[ip] = 0xB8;
					ip += 1;
					MoveMemory(&ThisMem[ip], &ptr_LoadLibraryA, 8);
					ip += 8;

					// call rax
					ThisMem[ip] = 0xFF;
					ip += 1;
					ThisMem[ip] = 0xD0;
					ip += 1;

					// 48 83 C4 28  add         rsp,28h - using 50h
					ThisMem[ip] = 0x48;
					ip += 1;
					ThisMem[ip] = 0x83;
					ip += 1;
					ThisMem[ip] = 0xC4;
					ip += 1;
					ThisMem[ip] = 0x50;
					ip += 1;

					// 59 pop rcx
					ThisMem[ip] = 0x59;
					ip += 1;

					// mov rax, old rip
					ThisMem[ip] = 0x48;
					ip += 1;
					ThisMem[ip] = 0xB8;
					ip += 1;
					MoveMemory(&ThisMem[ip], &ThreadEntryPoint, 8);
					ip += 8;

					// jmp rax
					ThisMem[ip] = 0xFF;
					ip += 1;
					ThisMem[ip] = 0xE0;
					ip += 1;

					if (!WriteProcessMemory(hProcess, PocessMem, ThisMem, 4096, NULL)) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};

					FlushInstructionCache(hProcess, PocessMem, 4096);

					Context.ContextFlags = CONTEXT_CONTROL;
					if (!GetThreadContext(hThread, &Context)) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};

					Context.Rip = (DWORD64)PocessMem;

					if (!SetThreadContext(hThread, &Context)) {
						VirtualFreeEx(hProcess, PocessMem, 0, MEM_RELEASE);
						return FALSE;
					};

					return TRUE;
				};
#endif

			};
		};
	};
};
