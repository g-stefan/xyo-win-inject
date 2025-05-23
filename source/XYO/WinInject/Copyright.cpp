// Win Inject
// Copyright (c) 2014-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/WinInject/Copyright.hpp>
#include <XYO/WinInject/Copyright.rh>

namespace XYO::Win::Inject::Copyright {

	static const char *copyright_ = XYO_WININJECT_COPYRIGHT;
	static const char *publisher_ = XYO_WININJECT_PUBLISHER;
	static const char *company_ = XYO_WININJECT_COMPANY;
	static const char *contact_ = XYO_WININJECT_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
