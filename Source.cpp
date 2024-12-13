#include <iostream>
#include <vector>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <tgbot/tgbot.h>
#include "FrontManager.h"

const std::string KEY = "7260187420:AAEWCDyZRteS7Y8ASvdZ7fHDZhYkPUxdzoU";

int main() {
	FrontManager f_manager(KEY);
	f_manager.start();
}