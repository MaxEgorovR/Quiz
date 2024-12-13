#pragma once
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

class APIManager {
public:
	nlohmann::json get_result(std::string url) {
		cpr::Response response = cpr::Get(cpr::Url{ url });
		if (response.status_code == 200) {
			return nlohmann::json::parse(response.text);
		}
		else {
			std::cout << "Error was occured: " << response.status_code;
			return nullptr;
		}
	}
};