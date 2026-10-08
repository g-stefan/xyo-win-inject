# XYO Win Inject — Documentation

`xyo-win-inject` is the DLL-injection and API-hooking support library of the
XYO C++ stack on Windows. It sits on top of `xyo-win` and does two related
jobs:

- **Process injection** (`XYO::Win::Inject::Process`). Start a program, or
  wrap a `CreateProcess*` call, so that a DLL of your choice is loaded into the
  new process *before its first instruction runs*. Injection works by creating
  the target suspended, writing a tiny stub into it that calls
  `LoadLibraryA(yourDll)`, pointing the primary thread at the stub, and then
  letting it continue into the program's real entry point.
- **Import Address Table (IAT) hooking** (`XYO::Win::Inject::Hook`). Once your
  DLL is inside the target, redirect the functions a module imports to your own
  replacements by rewriting entries in the import table — optionally across the
  whole module graph — and resolve names ↔ ordinals from a module's export
  table.

Together they are the building blocks of an *inject-and-hook* tool: a small
launcher injects a DLL into a program, and that DLL re-points selected API
calls (for example file, registry or network functions) at its own code.

```
xyo-version, xyo-cc, fabricare, applications ...
xyo-win-inject        <-- this library
xyo-win               (windows.h, Win32 helpers, StringCore)
xyo-system            (File, Shell, streams, ...)
xyo-encoding          (String, UTF, StringCore)
xyo-multithreading
xyo-data-structures
xyo-managed-memory
xyo-platform          (XYO_PLATFORM_32BIT / 64BIT, export macros)
```

## Purpose and intended use

DLL injection and IAT hooking are the standard Windows techniques behind
debuggers, profilers, test harnesses, compatibility shims, telemetry and
instrumentation layers, and plug-in loaders for programs you control or are
authorized to work on. This library packages those techniques so XYO tools —
notably [`dll-inject`](https://github.com/g-stefan/dll-inject) and its
sample — can launch a program with a DLL attached and let that DLL observe or
adjust the APIs the program uses.

The library does **not** bypass any security boundary: it needs the same rights
Windows already requires (you must be able to create or open the target process
with injection rights), and the injected DLL and the target must be the **same
bitness** (both 32-bit or both 64-bit). Use it only on processes you own or are
permitted to modify.

## Why use it

- **It does the awkward parts.** Suspended-create, cross-process allocation,
  thread-context hijacking, the 32- and 64-bit entry stubs, and the import-table
  walk are all handled for you.
- **Drop-in `CreateProcess*`.** `Process::createProcessA` / `W` /
  `createProcessAsUserA` / `W` take exactly the Win32 arguments plus a DLL path,
  so an existing launch site becomes an injecting launch site with one extra
  argument.
- **IAT hooking that follows the graph.** `Hook::processModule` rewrites a
  module and everything it imports (with a skip list and loop protection), so a
  hook reaches indirect callers too.
- **Part of the XYO stack.** Same `fabricare` build, same `Copyright` /
  `License` / `Version` metadata, same namespace layout as every other XYO
  library.

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Launch a program with a DLL injected | `Process::injectDll(cmdLine, dll)` | returns once the process is running |
| Launch and wait for it to finish | `Process::injectDllAndWait(cmdLine, dll)` | waits on the process's primary thread |
| Inject into an already-suspended process | `Process::injectDllDirect(hProcess, hThread, dll)` | you created it `CREATE_SUSPENDED` |
| `CreateProcess` + inject in one call | `Process::createProcessA/W(...)` | Win32 signature plus a trailing `const char *dllFile` |
| Same, as another user/token | `Process::createProcessAsUserA/W(hToken, ...)` | |
| Describe a function to hook | `Hook::setOriginalFunction(hook, module, name, newProc)` | fills a `HookProc` |
| Rewrite one module's IAT | `Hook::replaceFunction(hModule, hookList)` | `hookList` is a `nullptr`-terminated `HookProc*[]` |
| Rewrite a module and all it imports | `Hook::processModule(hModule, hookList, ...)` | with a processed list and a skip list |
| Hook-aware `GetProcAddress` | `Hook::getProcAddress(hModule, name, hookList)` | returns your `newProc` for hooked entries |
| Export name → ordinal | `Hook::getProcOrdinal(hModule, name)` | pass-through if already an ordinal |
| Export ordinal → name | `Hook::getProcName(hModule, ordinal)` | pass-through if already a name |
| List a module's imported DLLs | `Hook::enumImportTable(hModule, cb, userData)` | callback returns `FALSE` to stop |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build it, depend on it (DLL or static), a first inject-and-hook example, bitness and safety |
| [Process injection](injection.md) | `injectDll`, `injectDllAndWait`, `injectDllDirect`, the `createProcess*` wrappers, how injection works, return values |
| [IAT hooking](hooking.md) | `HookProc`, `setOriginalFunction`, `replaceFunction`, `processModule`, `getProcAddress`, name/ordinal lookup, `enumImportTable` |
| [API reference](reference.md) | Every public symbol on one page |

## Source map

```
source/XYO/WinInject.hpp                 umbrella header, include this
source/XYO/WinInject/
    Dependency.hpp                       xyo-win include, XYO_WININJECT_EXPORT macros
    Process[.cpp/.hpp]                   injectDll*, createProcess* wrappers, 32/64-bit stubs
    Hook[.cpp/.hpp]                      HookProc, IAT rewrite, export name/ordinal, enumImportTable
    Copyright / License / Version        library metadata
    Library.rc / Library.rh              version resource for the DLL
test/test.01.cpp                         export name <-> ordinal round trip (getProcOrdinal/getProcName)
test/test.02.cpp                         enumImportTable contract and metadata accessors
```

## AI assistant skill

A Claude Code skill summarizing how to use this library lives in
[`.claude/skills/xyo-win-inject/`](../.claude/skills/xyo-win-inject/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that depend on
`xyo-win-inject`.
