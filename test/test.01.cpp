// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2014-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Export resolution round trip for the IAT-hook helpers.
//
// getProcOrdinal / getProcName read the export directory of a module that is
// already loaded in this process, so the test needs no injection and no target
// process: it resolves well known KERNEL32 exports and checks that name ->
// ordinal -> name round trips and that the pass-through cases behave.

#include <XYO/WinInject.hpp>

#include <cstdio>
#include <cstring>
#include <stdexcept>

using namespace XYO::Win::Inject;

static void roundTrip(HMODULE hModule, const char *procName) {
	char name[256];
	strncpy(name, procName, sizeof(name) - 1);
	name[sizeof(name) - 1] = 0;

	// name -> ordinal
	LPSTR ordinal = Hook::getProcOrdinal(hModule, name);
	if (ordinal == 0) {
		throw std::runtime_error(std::string("getProcOrdinal returned 0 for ") + procName);
	};
	if (!IS_INTRESOURCE(ordinal)) {
		throw std::runtime_error(std::string("getProcOrdinal did not return an ordinal for ") + procName);
	};

	// ordinal -> name
	LPSTR back = Hook::getProcName(hModule, ordinal);
	if (back == 0) {
		throw std::runtime_error(std::string("getProcName returned 0 for ") + procName);
	};
	if (IS_INTRESOURCE(back)) {
		throw std::runtime_error(std::string("getProcName returned an ordinal for ") + procName);
	};
	if (strcmp(back, procName) != 0) {
		throw std::runtime_error(std::string("round trip mismatch: ") + procName + " -> " + back);
	};

	// pass through: a name stays a name, an ordinal stays the same ordinal
	if (Hook::getProcName(hModule, name) != name) {
		throw std::runtime_error("getProcName should return a name unchanged");
	};
	if (Hook::getProcOrdinal(hModule, ordinal) != ordinal) {
		throw std::runtime_error("getProcOrdinal should return an ordinal unchanged");
	};

	printf("  %-24s -> ordinal #%u -> %s\r\n", procName, (unsigned)(ULONG_PTR)ordinal, back);
};

void test() {
	HMODULE hKernel = GetModuleHandleA("KERNEL32.DLL");
	if (hKernel == nullptr) {
		throw std::runtime_error("GetModuleHandle KERNEL32.DLL");
	};

	roundTrip(hKernel, "Sleep");
	roundTrip(hKernel, "GetProcAddress");
	roundTrip(hKernel, "LoadLibraryA");
	roundTrip(hKernel, "VirtualAlloc");

	// A name that is not exported must resolve to 0.
	char missing[] = "ThisExportDoesNotExist_XYO";
	if (Hook::getProcOrdinal(hKernel, missing) != 0) {
		throw std::runtime_error("getProcOrdinal should return 0 for a missing export");
	};

	printf("Done.\r\n");
};

int main(int cmdN, char *cmdS[]) {

	try {
		test();
		return 0;
	} catch (const std::exception &e) {
		printf("* Error: %s\n", e.what());
	} catch (...) {
		printf("* Error: Unknown\n");
	};

	return 1;
};
