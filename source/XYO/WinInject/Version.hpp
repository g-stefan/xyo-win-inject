// Win Inject
// Copyright (c) 2014-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_WININJECT_VERSION_HPP
#define XYO_WININJECT_VERSION_HPP

#ifndef XYO_WININJECT_DEPENDENCY_HPP
#	include <XYO/WinInject/Dependency.hpp>
#endif

namespace XYO::Win::Inject::Version {

	XYO_WININJECT_EXPORT const char *version();
	XYO_WININJECT_EXPORT const char *build();
	XYO_WININJECT_EXPORT const char *versionWithBuild();
	XYO_WININJECT_EXPORT const char *datetime();

};

#endif
