# Getting started

## 1. Build and install

The library is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `xyo-platform`,
`xyo-managed-memory`, `xyo-data-structures`, `xyo-multithreading`,
`xyo-encoding`, `xyo-system` and `xyo-win` must be installed to the SDK first.
From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.*.cpp (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

Two libraries are produced:

| Project                   | Kind                                | Use it when                                    |
|---------------------------|-------------------------------------|------------------------------------------------|
| `xyo-win-inject`          | DLL / shared library (`dll-or-lib`) | default, shared between several components      |
| `xyo-win-inject.static`   | static library, static CRT          | a self-contained launcher `.exe` with no deps   |

This is a Windows-only library: it compiles under MSVC (and MinGW) for
`win32`/`win64`. There is no Linux build.

## 2. Depend on it from another fabricare project

In the consumer's `fabricare.json`:

```json
{
	"name": "my-injector",
	"make": "exe",
	"sourcePath": "XYO/MyInjector",
	"dependency": [
		"xyo-win-inject"
	]
}
```

Use `"xyo-win-inject.static"` instead for a static link. Then include the
umbrella header:

```cpp
#include <XYO/WinInject.hpp>

using namespace XYO::Win::Inject;   // Process, Hook, Copyright, License, Version
```

The umbrella header pulls in `<XYO/Win.hpp>` (and thus `<windows.h>`), the
export macros, and the `Process` and `Hook` declarations.

## 3. Bitness and privileges (read before you inject)

- **Bitness must match.** A 64-bit process can only load a 64-bit DLL and a
  32-bit process only a 32-bit DLL. Build the injected DLL and the launcher for
  the same architecture as the target. Injecting the wrong bitness makes
  `LoadLibrary` inside the target fail and the process is terminated.
- **The DLL path must be reachable by the target.** `injectDll*` passes the
  string to `LoadLibraryA` *inside the target process*, so use a full path or a
  name the target's search order can find. Keep it ASCII (the `A` APIs are
  used).
- **Rights.** Creating a process already grants the rights injection needs. To
  inject into a process you did not create you need a handle with
  `PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD`-class
  access and `THREAD_GET_CONTEXT | THREAD_SET_CONTEXT` on a thread; obtaining
  those may require matching integrity level or `SeDebugPrivilege`.
- **Only your own / authorized targets.** This is an instrumentation tool for
  software you control. Do not use it to modify processes you are not permitted
  to.

## 4. A minimal launcher: start a program with a DLL injected

```cpp
#include <XYO/WinInject.hpp>

using namespace XYO::Win::Inject;

int main() {
	char cmdLine[] = "C:\\Windows\\System32\\notepad.exe";
	if (!Process::injectDll(cmdLine, "C:\\hooks\\my-hook.dll")) {
		printf("injection failed: %lu\n", GetLastError());
		return 1;
	};
	// notepad is now running with my-hook.dll loaded.
	return 0;
}
```

`injectDll` creates the process suspended, injects the DLL, and resumes it,
returning as soon as the process is running. Use `injectDllAndWait` to block
until it exits.

> `cmdLine` is passed to `CreateProcessA`, whose contract allows it to modify
> the buffer in place — pass a writable `char` array, not a string literal.

## 5. Hooking an API from inside the injected DLL

Inside `my-hook.dll` (whose `DllMain` runs during the injected
`LoadLibraryA`), redirect an imported function to your own:

```cpp
#include <XYO/WinInject.hpp>

using namespace XYO::Win::Inject;

static Hook::HookProc hookMessageBox;
static Hook::HookProc *hookList[] = { &hookMessageBox, nullptr };

static int WINAPI myMessageBoxA(HWND h, LPCSTR text, LPCSTR caption, UINT type) {
	// ... observe or change, then call through ...
	typedef int (WINAPI *MessageBoxA_t)(HWND, LPCSTR, LPCSTR, UINT);
	return ((MessageBoxA_t)hookMessageBox.originalProc)(h, text, "hooked", type);
}

static void install() {
	Hook::setOriginalFunction(hookMessageBox, (LPSTR)"USER32.DLL",
	    (LPSTR)"MessageBoxA", (FARPROC)myMessageBoxA);

	// Rewrite the IAT of the main module and everything it imports,
	// skipping our own DLL and the module that owns the real function.
	HMODULE processed[256];
	size_t processedIndex = 0;
	LPSTR skip[] = { (LPSTR)"my-hook.dll", (LPSTR)"USER32.DLL", nullptr };
	Hook::processModule(GetModuleHandle(nullptr), hookList,
	    processed, processedIndex, 256, skip);
}
```

After `install()`, calls the target makes to `MessageBoxA` through its import
table land in `myMessageBoxA`, which can call the saved `originalProc` to reach
the real function. See [IAT hooking](hooking.md) for the full contract,
including `getProcAddress` for code that resolves functions dynamically.

## 6. Running the tests

```bash
fabricare make
fabricare test
```

`test.01` checks the export name ↔ ordinal round trip against KERNEL32, and
`test.02` checks the `enumImportTable` callback contract and the metadata
accessors. Both run entirely in-process and inject nothing, so they are safe to
run anywhere. See [`test/`](../test).

## 7. Building without fabricare

You can compile the sources directly; just provide the `xyo-win` /
`xyo-system` / ... include and library paths and define the export mode. For
the DLL build define `XYO_WIN_INJECT_DLL_INTERNAL` while compiling the library
and nothing special in the consumer (it will import). For a static build define
`XYO_WININJECT_LIBRARY` in both the library and the consumer so the
`XYO_WININJECT_EXPORT` macro expands to nothing. See
[`Dependency.hpp`](../source/XYO/WinInject/Dependency.hpp) for the exact macro
logic.
