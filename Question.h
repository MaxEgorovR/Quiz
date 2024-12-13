#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <tgbot/tgbot.h>

class Question {
public:
	std::string type;

	std::string difficulty;

	std::string category;

	std::string question;

	std::string correct_answer;
};