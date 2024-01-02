// Win Inject
// Copyright (c) 2014-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/WinInject/Version.hpp>
#include <XYO/WinInject/Version.rh>

namespace XYO::Win::Inject::Version {

	static const char *version_ = XYO_WININJECT_VERSION_STR;
	static const char *build_ = XYO_WININJECT_VERSION_STR_BUILD;
	static const char *versionWithBuild_ = XYO_WININJECT_VERSION_STR_WITH_BUILD;
	static const char *datetime_ = XYO_WININJECT_VERSION_STR_DATETIME;

	const char *version() {
		return version_;
	};
	const char *build() {
		return build_;
	};
	const char *versionWithBuild() {
		return versionWithBuild_;
	};
	const char *datetime() {
		return datetime_;
	};

};
