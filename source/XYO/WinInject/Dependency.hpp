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

#ifdef XYO_WIN_INJECT_INTERNAL
#	ifndef XYO_WININJECT_INTERNAL
#		define XYO_WININJECT_INTERNAL
#	endif
#endif

#ifdef XYO_WIN_INJECT_DLL_INTERNAL
#	ifndef XYO_WININJECT_DLL_INTERNAL
#		define XYO_WININJECT_DLL_INTERNAL
#	endif
#endif

#ifdef XYO_WININJECT_DLL_INTERNAL
#	define XYO_WININJECT_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_WININJECT_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_WININJECT_LIBRARY
#	undef XYO_WININJECT_EXPORT
#	define XYO_WININJECT_EXPORT
#endif
#ifdef XYO_WIN_INJECT_STATIC_LIB_INTERNAL
#	undef XYO_WININJECT_EXPORT
#	define XYO_WININJECT_EXPORT
#endif

// --

#endif
