// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2014-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Import table enumeration and library metadata.
//
// enumImportTable walks the import directory of a loaded module and calls back
// once per imported module name. This test runs it over this executable's own
// image (which imports at least KERNEL32) and checks both the "visit all" and
// the "stop early" callback contracts, then exercises the Copyright / License /
// Version metadata accessors.

#include <XYO/WinInject.hpp>

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

using namespace XYO::Win::Inject;

struct EnumState {
	int count;
	int stopAfter; // return FALSE once count reaches this (0 = never stop)
	bool sawKernel32;
};

static BOOL WINAPI onModule(PVOID userData, LPSTR module) {
	EnumState *state = (EnumState *)userData;
	++state->count;
	if (_stricmp(module, "KERNEL32.dll") == 0 || _stricmp(module, "KERNEL32.DLL") == 0) {
		state->sawKernel32 = true;
	};
	printf("  import[%d] = %s\r\n", state->count, module);
	if (state->stopAfter != 0 && state->count >= state->stopAfter) {
		return FALSE; // ask enumImportTable to stop
	};
	return TRUE; // continue
};

void test() {
	HMODULE hSelf = GetModuleHandleA(nullptr);
	if (hSelf == nullptr) {
		throw std::runtime_error("GetModuleHandle(self)");
	};

	// Visit every imported module.
	EnumState all = {0, 0, false};
	Hook::enumImportTable(hSelf, onModule, &all);
	if (all.count == 0) {
		throw std::runtime_error("enumImportTable visited no modules");
	};
	if (!all.sawKernel32) {
		throw std::runtime_error("enumImportTable did not report KERNEL32");
	};

	// Stop-early contract: returning FALSE from the callback must stop the walk.
	if (all.count > 1) {
		EnumState one = {0, 1, false};
		Hook::enumImportTable(hSelf, onModule, &one);
		if (one.count != 1) {
			throw std::runtime_error("enumImportTable ignored the stop request");
		};
	};

	// Metadata accessors must return non-empty strings.
	if (Version::version() == nullptr || strlen(Version::version()) == 0) {
		throw std::runtime_error("Version::version() is empty");
	};
	if (Version::build() == nullptr || strlen(Version::build()) == 0) {
		throw std::runtime_error("Version::build() is empty");
	};
	if (Copyright::copyright() == nullptr || strlen(Copyright::copyright()) == 0) {
		throw std::runtime_error("Copyright::copyright() is empty");
	};
	if (License::shortLicense().empty()) {
		throw std::runtime_error("License::shortLicense() is empty");
	};

	printf("  version %s build %s\r\n", Version::version(), Version::build());
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
