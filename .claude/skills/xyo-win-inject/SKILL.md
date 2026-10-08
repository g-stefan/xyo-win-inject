---
name: xyo-win-inject
description: >-
  How to use the xyo-win-inject C++ library (namespace XYO::Win::Inject), the
  Windows DLL-injection and API-hooking support library of the XYO C++ stack on
  top of xyo-win: the Process functions injectDll / injectDllAndWait /
  injectDllDirect and the createProcessA / W / createProcessAsUserA / W wrappers
  that create a process suspended, write a LoadLibraryA stub, hijack the primary
  thread (Eip/Rip) and resume so a DLL loads before the entry point; and the
  Hook functions for Import Address Table (IAT) hooking - HookProc,
  setOriginalFunction, replaceFunction, processModule (recursive, with a
  processed list and a skip list), getProcAddress (hook-aware), getProcOrdinal /
  getProcName (export name <-> ordinal), enumImportTable. Use when writing or
  reviewing code that includes <XYO/WinInject.hpp>, depends on "xyo-win-inject"
  in fabricare.json, injects a DLL into a Windows process, rewrites an import
  table or hooks a Win32 API, uses any of these names, or when working inside
  the xyo-win-inject repository or tools built on it (dll-inject,
  dll-inject-sample).
---

# xyo-win-inject

Windows DLL-injection and IAT API-hooking library of the XYO C++ stack, on top
of `xyo-win` (which brings `<windows.h>`, Win32 helpers and `StringCore`; its
rules and the `xyo-system` / `xyo-platform` skills apply). Purpose: **start a
process (or wrap `CreateProcess*`) so a DLL loads before the program's first
instruction, then from inside that DLL redirect imported API calls to your own
functions by rewriting the Import Address Table.**

Windows only (MSVC / MinGW, `win32` / `win64`). No Linux build.

Full documentation: `docs/` in this repository
(`X:\Storage\XYO\Gitea\CPP\xyo-win-inject\docs` on this machine): README,
getting-started, **injection**, **hooking**, reference. Read the matching page
when you need more than this summary. When in doubt read the header in
`source/XYO/WinInject/`.

## Scope / safety

Legitimate instrumentation technique (debuggers, profilers, test harnesses,
compat shims, plug-in loaders) for processes you own or are authorized to
modify. It grants no new privilege — it needs the rights Windows already
requires. Two hard rules the code cannot enforce for you:

- **Bitness must match**: a 64-bit target loads only a 64-bit DLL, 32-bit only
  32-bit. Wrong bitness ⇒ `LoadLibrary` fails in the target and the
  `createProcess*` wrappers terminate it.
- **The DLL path is `LoadLibraryA`-ed inside the target**: pass an ASCII full
  path (or a name the target's search order finds).

## API

```cpp
#include <XYO/WinInject.hpp>
using namespace XYO::Win::Inject;   // Process, Hook, Copyright, License, Version
```

### Process — create + inject

```cpp
BOOL Process::injectDll(char *cmdLine, const char *dllFile);        // start, inject, resume, return
BOOL Process::injectDllAndWait(char *cmdLine, const char *dllFile); // ... then wait on primary thread
BOOL Process::injectDllDirect(HANDLE hProcess, HANDLE hThread, const char *dllFile); // existing suspended proc; does NOT resume
BOOL Process::createProcessA (/*CreateProcessA args*/,        const char *dllFile);
BOOL Process::createProcessW (/*CreateProcessW args*/,        const char *dllFile);
BOOL Process::createProcessAsUserA(HANDLE hToken, /*...A*/,   const char *dllFile);
BOOL Process::createProcessAsUserW(HANDLE hToken, /*...W*/,   const char *dllFile);
```

- `cmdLine` is passed to `CreateProcessA` → **writable `char` buffer**, not a
  literal.
- The `createProcess*` wrappers = the Win32 call + a trailing `dllFile`. They OR
  in `CREATE_SUSPENDED`, inject, then resume **unless you passed
  `CREATE_SUSPENDED`** (then you resume). Injection failure ⇒ `TerminateProcess`
  + `FALSE`.
- How it works: `VirtualAllocEx` a page, save the thread's `Eip`/`Rip`, write a
  stub that `LoadLibraryA(dllFile)` then jumps back to the saved entry point,
  set the thread context to the stub, resume. 32- and 64-bit stubs via
  `XYO_PLATFORM_32BIT` / `_64BIT`.
- All return `BOOL`; on failure check `GetLastError()`.

### Hook — IAT hooking (run from inside the injected DLL)

```cpp
Hook::HookProc h;                              // { hModule, newProc, originalProc, procName, procOrdinal }
Hook::HookProc *hookList[] = { &h, nullptr };  // nullptr-terminated

Hook::setOriginalFunction(h, (LPSTR)"USER32.DLL", (LPSTR)"MessageBoxA", (FARPROC)myFn);
Hook::replaceFunction(hModule, hookList);      // rewrite ONE module's IAT
Hook::processModule(GetModuleHandle(nullptr), hookList, processed, idx, size, skip); // module + all imports, recursive
FARPROC p = Hook::getProcAddress(hModule, name, hookList);   // hook-aware: your newProc or nullptr
LPSTR ord = Hook::getProcOrdinal(hModule, (LPSTR)"Sleep");   // name -> MAKEINTRESOURCE ordinal (pass-through if ordinal, 0 if absent)
LPSTR nm  = Hook::getProcName(hModule, ord);                 // ordinal -> name (pass-through if name, 0 if none)
Hook::enumImportTable(hModule, cb, userData);                // cb(userData, moduleName) per import; return FALSE to stop
```

- `replaceFunction` compares each IAT slot against `originalProc` and, on a
  match, `VirtualProtect`s the slot writable, overwrites it with `newProc`, and
  restores protection. It only affects calls made **through the IAT of that
  module**; hook each module that matters, or use `processModule`.
- `processModule(hModule, hookList, processedList, processedListIndex,
  processedListSize, skipList)`: `processedList` is a caller `HMODULE[]` with
  `processedListIndex` starting at `0` (dedupe / recursion guard);
  `skipList` is a `nullptr`-terminated `LPSTR[]` of module names to skip —
  **always skip your own DLL** and usually the module owning the real function
  so your replacement can call through `originalProc`.
- For calls the target resolves dynamically, install a `GetProcAddress` hook
  that returns `Hook::getProcAddress(...)` when non-null, else the real one.
- `getProcOrdinal` / `getProcName` / `enumImportTable` are pure reads of a
  loaded module — no injection — so they are usable on their own.
- Keep `HookProc`s and the `hookList` alive for the hook's lifetime (static or
  heap, never stack).

### Metadata

```cpp
Copyright::copyright() / publisher() / company() / contact();   // const char *
License::license() / shortLicense();                            // std::string (MIT)
Version::version() / build() / versionWithBuild() / datetime(); // const char *
```

## Build (fabricare)

```bash
fabricare make      # output/ : xyo-win-inject (dll-or-lib) + xyo-win-inject.static (lib)
fabricare test      # build + run test/test.01 (name<->ordinal) and test.02 (enumImportTable, metadata); make first
fabricare install   # copy into ~/.fabricare/<platform>
fabricare clean
```

On this machine use platform `win64-msvc-2026`, and clear
`NoDefaultCurrentDirectoryInExePath` for the fabricare child process (see the
`fabricare` skill). Source files are **CRLF** — normalize new/edited files
before committing and check `git ls-files --eol` shows `w/crlf`.

Depend on it in a consumer `fabricare.json` with `"xyo-win-inject"` (DLL) or
`"xyo-win-inject.static"` (static). Export macros live in
`source/XYO/WinInject/Dependency.hpp`: `XYO_WININJECT_LIBRARY` for a static/
header build, `XYO_WIN_INJECT_DLL_INTERNAL` when building the DLL.

## Tests

`test/test.01.cpp` round-trips KERNEL32 exports through
`getProcOrdinal`/`getProcName` (and checks the pass-through and
not-found cases); `test/test.02.cpp` checks `enumImportTable`'s visit-all and
stop-early contract plus the metadata accessors. Both run in-process and inject
nothing, so they are safe anywhere. Add further tests as new `test/test.NN.cpp`
plus a `category: "test"` project in `fabricare.json` depending on
`xyo-win-inject`.
