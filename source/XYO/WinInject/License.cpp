// Win Inject
// Copyright (c) 2014-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/WinInject/License.hpp>
#include <XYO/WinInject/Copyright.hpp>

namespace XYO::Win::Inject::License {

	std::string license() {
		std::string retV;
		retV += Platform::License::licenseMITHeader();
		retV += Copyright::copyright();
		retV += "\r\n";
		retV += Platform::License::licenseMITContent();
		return retV;
	};

	std::string shortLicense() {
		std::string retV;
		retV += Copyright::copyright();
		retV += "\r\n";
		retV += Platform::License::licenseMITShort();
		return retV;
	};

};
