// Win Inject
// Copyright (c) 2014-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_WININJECT_COPYRIGHT_HPP
#define XYO_WININJECT_COPYRIGHT_HPP

#ifndef XYO_WININJECT_DEPENDENCY_HPP
#	include <XYO/WinInject/Dependency.hpp>
#endif

namespace XYO::Win::Inject::Copyright {
	XYO_WININJECT_EXPORT const char *copyright();
	XYO_WININJECT_EXPORT const char *publisher();
	XYO_WININJECT_EXPORT const char *company();
	XYO_WININJECT_EXPORT const char *contact();
};

#endif
