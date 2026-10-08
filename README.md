# XYO Win Inject

Support library for inject/hook

C++ library (Windows)
- Start a process with a DLL injected before its first instruction
(`Process::injectDll`, `injectDllAndWait`, `injectDllDirect`), or wrap a
`CreateProcess*` call with one extra argument
(`createProcessA` / `W`, `createProcessAsUserA` / `W`).
- Hook Win32 APIs by rewriting a module's Import Address Table
(`Hook::setOriginalFunction`, `replaceFunction`, `processModule` across the
whole import graph), with a hook-aware `getProcAddress`.
- Read a module's export table and imports
(`getProcOrdinal` / `getProcName`, `enumImportTable`).

Built on `xyo-win`; used by `dll-inject` and `dll-inject-sample`. An
instrumentation tool for processes you own or are authorized to modify;
it grants no new privilege and the DLL must match the target's bitness.

## Documentation

- [Overview](docs/README.md) - purpose, design and safety
- [Getting started](docs/getting-started.md) - build, depend on it, first inject-and-hook example, bitness
- [Process injection](docs/injection.md) - `injectDll*`, the `createProcess*` wrappers, how injection works
- [IAT hooking](docs/hooking.md) - `HookProc`, `replaceFunction`, `processModule`, name/ordinal, `enumImportTable`
- [API reference](docs/reference.md)

A Claude Code skill for this library is in
[.claude/skills/xyo-win-inject](.claude/skills/xyo-win-inject/SKILL.md).

## License

Copyright (c) 2014-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
