#pragma once

#include <string>

class Segment {
public:
	static std::string SendHttpPostRequest(const std::string& url, const std::string& postData) {
		(void)url;
		(void)postData;
		return "";
	}

	static std::string GetResponse(const std::string& userName) {
		(void)userName;
		return "";
	}

	static std::string DoubleToString(double value) {
		return std::to_string(static_cast<int>(value));
	}

	static std::string DoubleToString2(double value) {
		return std::to_string(value);
	}

	static void Update() {
	}
};
