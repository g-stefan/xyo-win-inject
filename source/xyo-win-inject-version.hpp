//
// XYO Win Inject
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef XYO_WIN_INJECT_VERSION_HPP
#define XYO_WIN_INJECT_VERSION_HPP

#define XYO_WIN_INJECT_VERSION_ABCD                 1,8,0,19
#define XYO_WIN_INJECT_VERSION_STR                 "1.8.0"
#define XYO_WIN_INJECT_VERSION_STR_BUILD           "19"
#define XYO_WIN_INJECT_VERSION_STR_DATETIME        "2021-11-15 16:51:55"

#ifndef XYO_RC

#ifndef XYO_WIN_INJECT__EXPORT_HPP
#include "xyo-win-inject--export.hpp"
#endif

namespace XYO {
	namespace Win {
		namespace Inject {
			namespace Version {
				XYO_WIN_INJECT_EXPORT const char *version();
				XYO_WIN_INJECT_EXPORT const char *build();
				XYO_WIN_INJECT_EXPORT const char *versionWithBuild();
				XYO_WIN_INJECT_EXPORT const char *datetime();
			};
		};
	};
};

#endif
#endif

