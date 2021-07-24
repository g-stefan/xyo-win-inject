//
// XYO Win Inject
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "xyo-win-inject-version.hpp"

namespace XYO {
	namespace Win {
		namespace Inject {
			namespace Version {

				static const char *version_ = "1.6.0";
				static const char *build_ = "8";
				static const char *versionWithBuild_ = "1.6.0.8";
				static const char *datetime_ = "2021-07-23 16:34:05";

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
		};
	};
};


