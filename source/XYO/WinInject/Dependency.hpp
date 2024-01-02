// Win Inject
// Copyright (c) 2014-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_WININJECT_DEPENDENCY_HPP
#define XYO_WININJECT_DEPENDENCY_HPP

#ifndef XYO_WIN_HPP
#	include <XYO/Win.hpp>
#endif

// -- Export

#ifndef XYO_WININJECT_INTERNAL
#	ifdef XYO_WIN_INJECT_INTERNAL
#		define XYO_WININJECT_INTERNAL
#	endif
#endif

#ifdef XYO_WININJECT_INTERNAL
#	define XYO_WININJECT_EXPORT XYO_LIBRARY_EXPORT
#else
#	define XYO_WININJECT_EXPORT XYO_LIBRARY_IMPORT
#endif

// --

#endif
