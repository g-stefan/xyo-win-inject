//
// XYO Win Inject
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "xyo-win-inject-version.hpp"

namespace XYO {
	namespace Win {
		namespace Inject {
			namespace Version {

				static const char *version_ = "1.10.0";
				static const char *build_ = "20";
				static const char *versionWithBuild_ = "1.10.0.20";
				static const char *datetime_ = "2022-01-01 21:19:47";

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


