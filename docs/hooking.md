# IAT hooking

`namespace XYO::Win::Inject::Hook` — redirect the functions a module imports to
your own replacements by rewriting its **Import Address Table (IAT)**, and read
name ↔ ordinal information from a module's **export table**.

```cpp
#include <XYO/WinInject.hpp>
using namespace XYO::Win::Inject;
```

## What IAT hooking is (and is not)

When a module calls, say, `MessageBoxA`, the compiler emits a call through a
slot in that module's IAT. The loader fills each slot with the real address at
load time. **IAT hooking overwrites the slot** with the address of your
function, so the existing `call` lands in your code without touching the target
function's bytes.

Consequences to keep in mind:

- It affects **one module's view** of an import. If three modules import
  `MessageBoxA`, each has its own IAT slot — hook each module you care about
  (`processModule` does this across the import graph for you).
- It only catches calls made **through the import table**. Calls through a
  pointer the target obtained from `GetProcAddress` are not in the IAT — provide
  `Hook::getProcAddress` as a hooked `GetProcAddress` to cover those.
- Functions imported **by ordinal** are matched by ordinal; functions imported
  by name are matched by the bound address. `replaceFunction` compares the IAT
  slot's current value against your recorded `originalProc`, so it works for
  both as long as `setOriginalFunction` resolved the real address.

## HookProc

```cpp
typedef struct SHookProc {
	HANDLE  hModule;       // module that exports the real function
	FARPROC newProc;       // your replacement
	FARPROC originalProc;  // the real function's current address
	LPSTR   procName;      // exported name   (or MAKEINTRESOURCE ordinal)
	LPSTR   procOrdinal;   // exported ordinal (MAKEINTRESOURCE)
} HookProc;
```

You generally do not fill this by hand — call `setOriginalFunction`. A hook
*list* is a plain C array of `HookProc *` terminated by `nullptr`:

```cpp
Hook::HookProc a, b;
Hook::HookProc *hookList[] = { &a, &b, nullptr };
```

## setOriginalFunction

```cpp
void setOriginalFunction(HookProc &hook, LPSTR moduleName, LPSTR procName, FARPROC newProc);
```

Resolve the real function and populate `hook`:

- `GetModuleHandle(moduleName)`, falling back to `LoadLibrary(moduleName)` if
  the module is not loaded yet (so the owning DLL stays resident);
- `originalProc = GetProcAddress(hModule, procName)`;
- records both the name and the ordinal form of `procName`
  (via `getProcName` / `getProcOrdinal`) and your `newProc`.

If the module cannot be found, `hook` is left all-`nullptr` (so it simply never
matches). `procName` may be a name (`"MessageBoxA"`) or an ordinal
(`MAKEINTRESOURCEA(2)`).

```cpp
Hook::setOriginalFunction(a, (LPSTR)"USER32.DLL", (LPSTR)"MessageBoxA",
    (FARPROC)myMessageBoxA);
```

## replaceFunction

```cpp
void replaceFunction(HMODULE hModule, HookProc **hookList);
```

Walk the IAT of **`hModule` only**. For every imported slot whose current value
equals some `hookList[i]->originalProc`, flip the page to `PAGE_READWRITE` with
`VirtualProtect`, overwrite the slot with `hookList[i]->newProc`, and restore
the protection. Slots that match no hook are untouched.

Call it once per module whose imports you want to redirect.

## processModule

```cpp
bool processModule(HMODULE hModule, HookProc **hookList,
                   HMODULE *processedList, size_t &processedListIndex,
                   size_t processedListSize, LPSTR *skipList);
```

`replaceFunction` across `hModule` **and, recursively, every module it
imports** — so a hook also catches a helper DLL that forwards to the target
API.

- `processedList` / `processedListIndex` / `processedListSize` — a caller-owned
  array used to remember which modules were already visited, preventing
  infinite recursion and repeated work. Start the index at `0`; size it to the
  most modules you expect (e.g. `256`).
- `skipList` — a `nullptr`-terminated array of module names **not** to rewrite.
  Always skip your own injected DLL and usually the module that owns the real
  function, so your replacement can still call through to it.

```cpp
HMODULE processed[256];
size_t  processedIndex = 0;
LPSTR   skip[] = { (LPSTR)"my-hook.dll", (LPSTR)"USER32.DLL", nullptr };

Hook::processModule(GetModuleHandle(nullptr), hookList,
    processed, processedIndex, 256, skip);
```

A module is marked processed before the skip check, so a skipped module is
still recorded (it will not be revisited). Returns `true` if it processed the
module, `false` if the module was null, already processed, over the list limit,
or on the skip list.

## getProcAddress (hook-aware)

```cpp
FARPROC getProcAddress(HMODULE hModule, LPCSTR lpProcName, HookProc **hookList);
```

Look `lpProcName` up **in your hook list** for module `hModule` and return the
matching `newProc`, or `nullptr` if it is not hooked. `lpProcName` may be a name
or an ordinal (`MAKEINTRESOURCE`). Use it to build a hooked `GetProcAddress`
replacement so code that resolves functions dynamically also gets your version:

```cpp
static FARPROC WINAPI myGetProcAddress(HMODULE h, LPCSTR name) {
	FARPROC hooked = Hook::getProcAddress(h, name, hookList);
	if (hooked) {
		return hooked;
	};
	return GetProcAddress(h, name);   // the real one (saved via a hook of its own)
}
```

## Export name ↔ ordinal

```cpp
LPSTR getProcOrdinal(HMODULE hModule, LPSTR procName);    // name -> ordinal
LPSTR getProcName   (HMODULE hModule, LPSTR procOrdinal); // ordinal -> name
```

Read `hModule`'s export directory to convert between a function's exported name
and its ordinal:

- `getProcOrdinal` returns the ordinal as `MAKEINTRESOURCEA(...)`; if `procName`
  is *already* an ordinal (`IS_INTRESOURCE`), it is returned unchanged; `0` if
  the name is not exported.
- `getProcName` returns the exported name string; if `procOrdinal` is *already*
  a name, it is returned unchanged; `0` if that ordinal has no name.

They are pure lookups over an already-loaded module and perform no injection, so
they are also handy on their own. `test/test.01.cpp` round-trips KERNEL32
exports through both.

## enumImportTable

```cpp
typedef BOOL (WINAPI *IEnumImportModuleName)(PVOID userData, LPSTR module);
BOOL enumImportTable(HMODULE hModule, IEnumImportModuleName iEnum, PVOID userData);
```

Call `iEnum(userData, moduleName)` once for each DLL `hModule` imports. The
callback returns `TRUE` to continue or `FALSE` to stop the enumeration early.
Use it to discover what to hook, or to decide a skip list.

```cpp
static BOOL WINAPI printImport(PVOID, LPSTR module) {
	printf("imports %s\n", module);
	return TRUE;
}
Hook::enumImportTable(GetModuleHandle(nullptr), printImport, nullptr);
```

`test/test.02.cpp` checks both the visit-all and the stop-early behavior.

## A complete hook, end to end

1. Inject your DLL with [`Process::injectDll`](injection.md) (or a
   `createProcess*` wrapper).
2. In `DllMain(DLL_PROCESS_ATTACH)`, build your `HookProc`s with
   `setOriginalFunction` and a `nullptr`-terminated `hookList`.
3. `processModule(GetModuleHandle(nullptr), hookList, processed, idx, size,
   skip)` to rewrite the target and its import graph.
4. In each replacement, do your work and call through `hook.originalProc`.
5. Optionally hook `GetProcAddress` itself and answer from
   `Hook::getProcAddress` so dynamic resolution is covered too.

Keep your `HookProc`s and `hookList` alive for the lifetime of the hook (static
or heap, not stack).
