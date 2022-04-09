//
// XYO Win Inject
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef XYO_WIN_INJECT_COPYRIGHT_HPP
#define XYO_WIN_INJECT_COPYRIGHT_HPP

#define XYO_WIN_INJECT_COPYRIGHT "Copyright (c) Grigore Stefan"
#define XYO_WIN_INJECT_PUBLISHER "Grigore Stefan"
#define XYO_WIN_INJECT_COMPANY XYO_WIN_INJECT_PUBLISHER
#define XYO_WIN_INJECT_CONTACT "g_stefan@yahoo.com"
#define XYO_WIN_INJECT_FULL_COPYRIGHT XYO_WIN_INJECT_COPYRIGHT " <" XYO_WIN_INJECT_CONTACT ">"

#ifndef XYO_RC

#	ifndef XYO_WIN_INJECT__EXPORT_HPP
#		include "xyo-win-inject--export.hpp"
#	endif

namespace XYO {
	namespace Win {
		namespace Inject {
			namespace Copyright {
				XYO_WIN_INJECT_EXPORT const char *copyright();
				XYO_WIN_INJECT_EXPORT const char *publisher();
				XYO_WIN_INJECT_EXPORT const char *company();
				XYO_WIN_INJECT_EXPORT const char *contact();
				XYO_WIN_INJECT_EXPORT const char *fullCopyright();
			};
		};
	};
};

#endif
#endif
