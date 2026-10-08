# API reference

Every public symbol of `xyo-win-inject`. Include `<XYO/WinInject.hpp>`; all
names live under `namespace XYO::Win::Inject`. Every declaration is marked
`XYO_WININJECT_EXPORT`.

```cpp
#include <XYO/WinInject.hpp>
using namespace XYO::Win::Inject;   // Process, Hook, Copyright, License, Version
```

## Process — `XYO::Win::Inject::Process`

| Signature | Returns | Summary |
|-----------|---------|---------|
| `BOOL injectDll(char *cmdLine, const char *dllFile)` | `TRUE`/`FALSE` | Start `cmdLine` suspended, inject `dllFile`, resume, return. |
| `BOOL injectDllAndWait(char *cmdLine, const char *dllFile)` | `TRUE`/`FALSE` | As `injectDll`, then wait on the primary thread before returning. |
| `BOOL injectDllDirect(HANDLE hProcess, HANDLE hThread, const char *dllFile)` | `TRUE`/`FALSE` | Inject into an already-suspended process by hijacking `hThread`; does **not** resume. |
| `BOOL createProcessA(LPCTSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCTSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, const char *dllFile)` | `TRUE`/`FALSE` | `CreateProcessA` + inject; resumes unless you passed `CREATE_SUSPENDED`. |
| `BOOL createProcessW(... LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, const char *dllFile)` | `TRUE`/`FALSE` | Wide-char variant of `createProcessA`. |
| `BOOL createProcessAsUserA(HANDLE hToken, ... const char *dllFile)` | `TRUE`/`FALSE` | `CreateProcessAsUserA` + inject. |
| `BOOL createProcessAsUserW(HANDLE hToken, ... const char *dllFile)` | `TRUE`/`FALSE` | `CreateProcessAsUserW` + inject. |

The `createProcess*` wrappers take exactly the arguments of the Win32 function
of the same name, plus a trailing `const char *dllFile`. They OR
`CREATE_SUSPENDED` into `dwCreationFlags` internally; if injection fails they
`TerminateProcess` the target and return `FALSE`. See
[Process injection](injection.md).

## Hook — `XYO::Win::Inject::Hook`

### Types

```cpp
typedef struct SHookProc {
	HANDLE  hModule;       // module exporting the real function
	FARPROC newProc;       // replacement
	FARPROC originalProc;  // real function's current address
	LPSTR   procName;      // exported name (or MAKEINTRESOURCE ordinal)
	LPSTR   procOrdinal;   // exported ordinal (MAKEINTRESOURCE)
} HookProc;

typedef BOOL (WINAPI *IEnumImportModuleName)(PVOID userData, LPSTR module);
```

A **hook list** is a `nullptr`-terminated array of `HookProc *`.

### Functions

| Signature | Returns | Summary |
|-----------|---------|---------|
| `void setOriginalFunction(HookProc &hook, LPSTR moduleName, LPSTR procName, FARPROC newProc)` | — | Fill `hook`: resolve module (load if needed), `originalProc`, name & ordinal, store `newProc`. All-`nullptr` if the module is not found. |
| `void replaceFunction(HMODULE hModule, HookProc **hookList)` | — | Rewrite `hModule`'s IAT slots whose value equals a hook's `originalProc` to that hook's `newProc`. |
| `bool processModule(HMODULE hModule, HookProc **hookList, HMODULE *processedList, size_t &processedListIndex, size_t processedListSize, LPSTR *skipList)` | `bool` | `replaceFunction` on `hModule` and, recursively, every module it imports. `processedList` dedupes; `skipList` (`nullptr`-terminated names) is skipped. `false` if null/already-done/over-limit/skipped. |
| `FARPROC getProcAddress(HMODULE hModule, LPCSTR lpProcName, HookProc **hookList)` | `FARPROC` | The `newProc` registered for `(hModule, lpProcName)` in `hookList`, or `nullptr`. Name or ordinal. |
| `LPSTR getProcOrdinal(HMODULE hModule, LPSTR procName)` | `LPSTR` | Exported name → ordinal (`MAKEINTRESOURCEA`). Pass-through if already an ordinal; `0` if not exported. |
| `LPSTR getProcName(HMODULE hModule, LPSTR procOrdinal)` | `LPSTR` | Exported ordinal → name. Pass-through if already a name; `0` if the ordinal has no name. |
| `BOOL enumImportTable(HMODULE hModule, IEnumImportModuleName iEnum, PVOID userData)` | `BOOL` | Call `iEnum(userData, moduleName)` per imported DLL; callback returns `FALSE` to stop. |

See [IAT hooking](hooking.md).

## Copyright — `XYO::Win::Inject::Copyright`

| Signature | Summary |
|-----------|---------|
| `const char *copyright()` | Copyright line. |
| `const char *publisher()` | Publisher name. |
| `const char *company()` | Company (same as publisher). |
| `const char *contact()` | Contact e-mail. |

## License — `XYO::Win::Inject::License`

| Signature | Summary |
|-----------|---------|
| `std::string license()` | Full MIT license text with the copyright header. |
| `std::string shortLicense()` | Copyright line plus the short MIT notice. |

## Version — `XYO::Win::Inject::Version`

| Signature | Summary |
|-----------|---------|
| `const char *version()` | Version, e.g. `"4.9.0"`. |
| `const char *build()` | Build number, e.g. `"6"`. |
| `const char *versionWithBuild()` | e.g. `"4.9.0.6"`. |
| `const char *datetime()` | Build date-time, e.g. `"2026-09-16 23:03:11"`. |

## Build-time macros (`Dependency.hpp`)

| Macro | Effect |
|-------|--------|
| `XYO_WININJECT_EXPORT` | Expands to the platform DLL import/export attribute, or nothing for a static/library build. |
| `XYO_WIN_INJECT_DLL_INTERNAL` / `XYO_WININJECT_DLL_INTERNAL` | Define when **building** the DLL (export). |
| `XYO_WININJECT_LIBRARY` | Define in both library and consumer for a header/static build (no import/export). |
| `XYO_WIN_INJECT_STATIC_LIB_INTERNAL` | Static-library internal build (no export). |
| `XYO_PLATFORM_32BIT` / `XYO_PLATFORM_64BIT` | From `xyo-platform`; selects the 32- or 64-bit injection stub. |
