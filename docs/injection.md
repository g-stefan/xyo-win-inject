# Process injection

`namespace XYO::Win::Inject::Process` — start a process (or wrap a
`CreateProcess*` call) so that a DLL is loaded into it before its first
instruction runs, or inject into a process you already created suspended.

```cpp
#include <XYO/WinInject.hpp>
using namespace XYO::Win::Inject;
```

## How injection works

All of the entry points below funnel into `injectDllDirect`, which operates on
a **suspended** process and its **primary thread**:

1. `VirtualAllocEx` a 4 KiB page in the target, read/write.
2. Read the primary thread's context and save its instruction pointer
   (`Eip` on 32-bit, `Rip` on 64-bit) — the program's real entry point.
3. Build a small machine-code stub in that page:
   - push / set the DLL path string (also written into the page),
   - call `LoadLibraryA` (resolved to the same address KERNEL32 has in this
     process; it is identical in the target),
   - restore registers and **jump back to the saved entry point**.
4. Flip the page to execute-read, flush the instruction cache, and set the
   thread's instruction pointer to the stub.
5. When the thread is resumed, the stub loads the DLL (running its `DllMain`),
   then continues into the program exactly as if nothing happened.

Because the stub runs on the existing primary thread before the program starts,
the DLL is present for the whole lifetime of the process. Separate 32-bit and
64-bit stubs are compiled in via `XYO_PLATFORM_32BIT` / `XYO_PLATFORM_64BIT`.

If any step fails, the allocated page is freed; and for the `createProcess*`
wrappers a failed injection **terminates the target** and the call returns
`FALSE`, so you never get a half-injected process.

## injectDll

```cpp
BOOL injectDll(char *cmdLine, const char *dllFile);
```

Create the process described by `cmdLine` (via `createProcessA` with no
application name), inject `dllFile`, resume, and return. `TRUE` on success.

- `cmdLine` is handed to `CreateProcessA`, which may write into the buffer, so
  pass a writable `char` array.
- Returns as soon as the process is running; the injected `DllMain` has run by
  then (it runs on the primary thread before the real entry point).

```cpp
char cmd[] = "target.exe --flag";
Process::injectDll(cmd, "C:\\hooks\\hook.dll");
```

## injectDllAndWait

```cpp
BOOL injectDllAndWait(char *cmdLine, const char *dllFile);
```

Same as `injectDll`, but after resuming it blocks on the process's primary
thread (`WaitForSingleObject(..., INFINITE)`) before returning — use it for a
batch-style launcher that should not return until the launched program is done.

## injectDllDirect

```cpp
BOOL injectDllDirect(HANDLE hProcess, HANDLE hThread, const char *dllFile);
```

The primitive the others are built on. Inject `dllFile` into `hProcess` by
hijacking `hThread`. **Preconditions:**

- `hThread` is the process's primary thread and the process is **suspended**
  (created with `CREATE_SUSPENDED`, before `ResumeThread`).
- `hProcess` has VM-operation / VM-write / context rights; `hThread` has
  get/set-context rights.

It does **not** resume the thread — the caller does that once injection
succeeds. Returns `FALSE` (and frees its scratch page) on any failure.

```cpp
STARTUPINFOA si = { sizeof(si) };
PROCESS_INFORMATION pi = {};
if (CreateProcessA(nullptr, cmd, nullptr, nullptr, FALSE,
        CREATE_SUSPENDED, nullptr, nullptr, &si, &pi)) {
	if (Process::injectDllDirect(pi.hProcess, pi.hThread, "hook.dll")) {
		ResumeThread(pi.hThread);
	} else {
		TerminateProcess(pi.hProcess, 0);
	};
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
```

## The CreateProcess wrappers

Four wrappers mirror the Win32 functions exactly, with **one extra trailing
argument**, `const char *dllFile`:

```cpp
BOOL createProcessA       (/* CreateProcessA args */,        const char *dllFile);
BOOL createProcessW       (/* CreateProcessW args */,        const char *dllFile);
BOOL createProcessAsUserA (HANDLE hToken, /* ...A args */,   const char *dllFile);
BOOL createProcessAsUserW (HANDLE hToken, /* ...W args */,   const char *dllFile);
```

Each one:

1. forces `CREATE_SUSPENDED` on top of your `dwCreationFlags` and calls the
   corresponding Win32 `CreateProcess*`,
2. on success calls `injectDllDirect`,
3. then **resumes the primary thread** — *unless you yourself asked for
   `CREATE_SUSPENDED`*, in which case it is left suspended for you to resume
   (so the wrappers are transparent: pass `CREATE_SUSPENDED` and you still own
   the resume),
4. if injection fails, terminates the process and returns `FALSE`.

So converting an existing launch site to an injecting one is a one-argument
change:

```cpp
// before
CreateProcessW(app, cmd, 0, 0, FALSE, flags, 0, 0, &si, &pi);
// after
Process::createProcessW(app, cmd, 0, 0, FALSE, flags, 0, 0, &si, &pi, L_dll_ignored);
```

> Note: the wrappers take the process/thread **`STARTUPINFO`** and receive the
> **`PROCESS_INFORMATION`** just like the Win32 calls; on success the handles in
> `PROCESS_INFORMATION` are yours to `CloseHandle`.

## Return values and errors

All functions return `BOOL` (`TRUE`/`FALSE`). On failure the underlying Win32
error is available from `GetLastError()` for the step that failed
(`CreateProcess*`, `VirtualAllocEx`, `GetThreadContext`, `WriteProcessMemory`,
`VirtualProtectEx`, `SetThreadContext`). A `FALSE` from a `createProcess*`
wrapper means either the create failed (no process was started) or injection
failed (the started process was terminated).

## Checklist

- Target and DLL are the **same bitness**.
- `dllFile` is an ASCII path the **target** can `LoadLibraryA`.
- For `injectDllDirect`, the process is **suspended** and you resume it after a
  `TRUE`.
- Pass a **writable** command-line buffer.
- Close the `PROCESS_INFORMATION` handles you receive.
