#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <tgbot/tgbot.h>
#include "APIManager.h"
#include "Question.h"
#include <map>


class FrontManager {
public:
	FrontManager(std::string KEY)
	{
		this->KEY = KEY;
	}

	void start() {
		TgBot::Bot bot(this->KEY);

		TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
		std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
		std::vector<TgBot::InlineKeyboardButton::Ptr> row1;
		TgBot::InlineKeyboardButton::Ptr oneButton(new TgBot::InlineKeyboardButton);
		oneButton->text = "random";
		oneButton->callbackData = "1";
		row0.push_back(oneButton);

		TgBot::InlineKeyboardButton::Ptr twoButton(new TgBot::InlineKeyboardButton);
		twoButton->text = "easy";
		twoButton->callbackData = "2";
		row0.push_back(twoButton);

		TgBot::InlineKeyboardButton::Ptr threeButton(new TgBot::InlineKeyboardButton);
		threeButton->text = "medium";
		threeButton->callbackData = "3";
		row1.push_back(threeButton);

		TgBot::InlineKeyboardButton::Ptr fourButton(new TgBot::InlineKeyboardButton);
		fourButton->text = "hard";
		fourButton->callbackData = "4";
		row1.push_back(fourButton);
		keyboard->inlineKeyboard.push_back(row0);
		keyboard->inlineKeyboard.push_back(row1);

		TgBot::InlineKeyboardMarkup::Ptr keyboard1(new TgBot::InlineKeyboardMarkup);
		std::vector<TgBot::InlineKeyboardButton::Ptr> row2;
		TgBot::InlineKeyboardButton::Ptr fiveButton(new TgBot::InlineKeyboardButton);
		fiveButton->text = "multiple";
		fiveButton->callbackData = "6";
		row2.push_back(fiveButton);

		TgBot::InlineKeyboardButton::Ptr sixButton(new TgBot::InlineKeyboardButton);
		sixButton->text = "Ture/False";
		sixButton->callbackData = "7";
		row2.push_back(sixButton);

		TgBot::InlineKeyboardButton::Ptr sevenButton(new TgBot::InlineKeyboardButton);
		sevenButton->text = "random";
		sevenButton->callbackData = "5";
		row2.push_back(sevenButton);
		keyboard1->inlineKeyboard.push_back(row2);

		bot.getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
			bot.getApi().sendMessage(message->chat->id, "1 - Get 10 random questions\n2 - Get 10 questions with easy difficulty\n3 - Get 10 questions with medium difficulty\n4 - Get 10 questions with hard difficulty\n", 0, 0, keyboard);
			});

		bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
			this->accuracy = 0;
			if (StringTools::startsWith(message->text, "/start")) {
				return;
			}
			else {
				std::string tmp;
				answers = this->split(message->text, '"');
				for (int i = 0;i < answers.size()&& i < questions.size(); i++) {
					std::cout << answers[i];
					std::cout<<" " + questions[i].correct_answer;
					if (std::string(questions[i].correct_answer) == std::string(answers[i])) {
						accuracy++;
					}
				}
				std::cout << "::::"<< accuracy;
				double tmp1 = this->accuracy / 10.0 * 100.0;
				int five_point_sys = int(tmp1 / 100.0 * 5.0);
				int twelve_point_sys = int(tmp1 / 100.0 * 12.0);
				tmp = "Accuracy: " + std::to_string(tmp1) + "%\nRating in a five-point system: " + std::to_string(five_point_sys) + "\nRating in a twelve-point system: " + std::to_string(twelve_point_sys) + "\n";
				bot.getApi().sendMessage(message->chat->id, tmp, 0, 0);
			}
			});

		bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
			if (StringTools::startsWith(query->data, "1")) {
				std::cout << query->data;
				questions = this->getRandom();
				std::string tmp = "";
				for (int i = 0;i < 10;i++) {
					tmp += "Category: " + questions[i].category + "\n" +
						"Difficulty: " + questions[i].difficulty + "\n"+
						"Type: "+questions[i].type+"\n"+
						"Question: " + questions[i].question + "\n\n";
				}
				bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
			}
			else if (StringTools::startsWith(query->data, "2")) {
				std::cout << query->data;
				bot.getApi().sendMessage(query->message->chat->id, "Select type of questions", 0, 0, keyboard1);
				bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
					if (StringTools::startsWith(query->data, "5")) {
						std::vector<Question> questions = this->getEasy(-1);
						std::string tmp = "";
						for (int i = 0;i < 10;i++) {
							tmp += "Category: " + questions[i].category + "\n" +
								"Difficulty: " + questions[i].difficulty + "\n" +
								"Type: " + questions[i].type + "\n" +
								"Question: " + questions[i].question + "\n\n";
						}
						bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
					}
					if (StringTools::startsWith(query->data, "6")) {
						std::vector<Question> questions = this->getEasy(0);
						std::string tmp = "";
						for (int i = 0;i < 10;i++) {
							tmp += "Category: " + questions[i].category + "\n" +
								"Difficulty: " + questions[i].difficulty + "\n" +
								"Type: " + questions[i].type + "\n" +
								"Question: " + questions[i].question + "\n\n";
						}
						bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
					}
					if (StringTools::startsWith(query->data, "7")) {
						std::vector<Question> questions = this->getEasy(1);
						std::string tmp = "";
						for (int i = 0;i < 10;i++) {
							tmp += "Category: " + questions[i].category + "\n" +
								"Difficulty: " + questions[i].difficulty + "\n" +
								"Type: " + questions[i].type + "\n" +
								"Question: " + questions[i].question + "\n\n";
						}
						bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
					}
					});
			}
			else if (StringTools::startsWith(query->data, "3")) {
			std::cout << query->data;
				bot.getApi().sendMessage(query->message->chat->id, "Select type of questions", 0, 0, keyboard1);
				bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
					if (StringTools::startsWith(query->data, "5")) {
						std::vector<Question> questions = this->getMedium(-1);
						std::string tmp = "";
						for (int i = 0;i < 10;i++) {
							tmp += "Category: " + questions[i].category + "\n" +
								"Difficulty: " + questions[i].difficulty + "\n" +
								"Type: " + questions[i].type + "\n" +
								"Question: " + questions[i].question + "\n\n";
						}
						bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
					}
					if (StringTools::startsWith(query->data, "6")) {
						std::vector<Question> questions = this->getMedium(0);
						std::string tmp = "";
						for (int i = 0;i < 10;i++) {
							tmp += "Category: " + questions[i].category + "\n" +
								"Difficulty: " + questions[i].difficulty + "\n" +
								"Type: " + questions[i].type + "\n" +
								"Question: " + questions[i].question + "\n\n";
						}
						bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
					}
					if (StringTools::startsWith(query->data, "7")) {
						std::vector<Question> questions = this->getMedium(1);
						std::string tmp = "";
						for (int i = 0;i < 10;i++) {
							tmp += "Category: " + questions[i].category + "\n" +
								"Difficulty: " + questions[i].difficulty + "\n" +
								"Type: " + questions[i].type + "\n" +
								"Question: " + questions[i].question + "\n\n";
						}
						bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
					}
					});
			}
			else if (StringTools::startsWith(query->data, "4")) {
			std::cout << query->data;
				bot.getApi().sendMessage(query->message->chat->id, "Select type of questions", 0, 0, keyboard1);
				bot.getEvents().onCallbackQuery([&](TgBot::CallbackQuery::Ptr query) {
					if (StringTools::startsWith(query->data, "5")) {
						std::vector<Question> questions = this->getHard(-1);
						std::string tmp = "";
						for (int i = 0;i < 10;i++) {
							tmp += "Category: " + questions[i].category + "\n" +
								"Difficulty: " + questions[i].difficulty + "\n" +
								"Type: " + questions[i].type + "\n" +
								"Question: " + questions[i].question + "\n\n";
						}
						bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
					}
					if (StringTools::startsWith(query->data, "6")) {
						std::vector<Question> questions = this->getHard(0);
						std::string tmp = "";
						for (int i = 0;i < 10;i++) {
							tmp += "Category: " + questions[i].category + "\n" +
								"Difficulty: " + questions[i].difficulty + "\n" +
								"Type: " + questions[i].type + "\n" +
								"Question: " + questions[i].question + "\n\n";
						}
						bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
					}
					if (StringTools::startsWith(query->data, "7")) {
						std::vector<Question> questions = this->getHard(1);
						std::string tmp = "";
						for (int i = 0;i < 10;i++) {
							tmp += "Category: " + questions[i].category + "\n" +
								"Difficulty: " + questions[i].difficulty + "\n" +
								"Type: " + questions[i].type + "\n" +
								"Question: " + questions[i].question + "\n\n";
						}
						bot.getApi().sendMessage(query->message->chat->id, tmp + "Answer split by forging", 0, 0);
					}
					});
			}
			});

		try {
			TgBot::TgLongPoll longPoll(bot);
			while (true) {
				printf("Long polling\n");
				longPoll.start();
			}
		}
		catch (std::exception ex) {
			std::cout << ex.what();
		}
	}

	std::vector<Question> getRandom() {
		std::vector<Question> questions;
		nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10");
		for (int i = 0;i < 10;i++) {
			this->question.type = std::string(response["results"][i]["type"]);
			this->question.difficulty = std::string(response["results"][i]["difficulty"]);
			this->question.category = std::string(response["results"][i]["category"]);
			this->question.question = std::string(response["results"][i]["question"]);
			this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
			questions.push_back(this->question);
		}
		std::cout << this->question.correct_answer;
		return questions;
	}

	std::vector<Question> getEasy(int flag) {
		std::vector<Question> questions;
		if (flag == 0) {
			nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10&difficulty=easy&type=multiple");
			for (int i = 0;i < 10;i++) {
				this->question.type = std::string(response["results"][i]["type"]);
				this->question.difficulty = std::string(response["results"][i]["difficulty"]);
				this->question.category = std::string(response["results"][i]["category"]);
				this->question.question = std::string(response["results"][i]["question"]);
				this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
				questions.push_back(this->question);
			}
			return questions;
		}
		else if (flag == 1) {
			nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10&difficulty=easy&type=boolean");
			for (int i = 0;i < 10;i++) {
				this->question.type = std::string(response["results"][i]["type"]);
				this->question.difficulty = std::string(response["results"][i]["difficulty"]);
				this->question.category = std::string(response["results"][i]["category"]);
				this->question.question = std::string(response["results"][i]["question"]);
				this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
				questions.push_back(this->question);
			}
			return questions;
		}
		else if (flag == -1) {
			nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10&difficulty=easy");
			for (int i = 0;i < 10;i++) {
				this->question.type = std::string(response["results"][i]["type"]);
				this->question.difficulty = std::string(response["results"][i]["difficulty"]);
				this->question.category = std::string(response["results"][i]["category"]);
				this->question.question = std::string(response["results"][i]["question"]);
				this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
				questions.push_back(this->question);
			}
			return questions;
		}
	}

	std::vector<Question> getMedium(int flag) {
		std::vector<Question> questions;
		if (flag == 0) {
			nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10&difficulty=easy&type=multiple");
			for (int i = 0;i < 10;i++) {
				this->question.type = std::string(response["results"][i]["type"]);
				this->question.difficulty = std::string(response["results"][i]["difficulty"]);
				this->question.category = std::string(response["results"][i]["category"]);
				this->question.question = std::string(response["results"][i]["question"]);
				this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
				questions.push_back(this->question);
			}
			return questions;
		}
		else if (flag == 1) {
			nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10&difficulty=easy&type=boolean");
			for (int i = 0;i < 10;i++) {
				this->question.type = std::string(response["results"][i]["type"]);
				this->question.difficulty = std::string(response["results"][i]["difficulty"]);
				this->question.category = std::string(response["results"][i]["category"]);
				this->question.question = std::string(response["results"][i]["question"]);
				this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
				questions.push_back(this->question);
			}
			return questions;
		}
		else if (flag == -1) {
			nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10&difficulty=easy");
			for (int i = 0;i < 10;i++) {
				this->question.type = std::string(response["results"][i]["type"]);
				this->question.difficulty = std::string(response["results"][i]["difficulty"]);
				this->question.category = std::string(response["results"][i]["category"]);
				this->question.question = std::string(response["results"][i]["question"]);
				this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
				questions.push_back(this->question);
			}
			return questions;
		}
	}

	std::vector<Question> getHard(int flag) {
		std::vector<Question> questions;
		if (flag == 0) {
			nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10&difficulty=easy&type=multiple");
			for (int i = 0;i < 10;i++) {
				this->question.type = std::string(response["results"][i]["type"]);
				this->question.difficulty = std::string(response["results"][i]["difficulty"]);
				this->question.category = std::string(response["results"][i]["category"]);
				this->question.question = std::string(response["results"][i]["question"]);
				this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
				questions.push_back(this->question);
			}
			return questions;
		}
		else if (flag == 1) {
			nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10&difficulty=easy&type=boolean");
			for (int i = 0;i < 10;i++) {
				this->question.type = std::string(response["results"][i]["type"]);
				this->question.difficulty = std::string(response["results"][i]["difficulty"]);
				this->question.category = std::string(response["results"][i]["category"]);
				this->question.question = std::string(response["results"][i]["question"]);
				this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
				questions.push_back(this->question);
			}
			return questions;
		}
		else if (flag == -1) {
			nlohmann::json response = a_manager->get_result("https://opentdb.com/api.php?amount=10&difficulty=easy");
			for (int i = 0;i < 10;i++) {
				this->question.type = std::string(response["results"][i]["type"]);
				this->question.difficulty = std::string(response["results"][i]["difficulty"]);
				this->question.category = std::string(response["results"][i]["category"]);
				this->question.question = std::string(response["results"][i]["question"]);
				this->question.correct_answer = std::string(response["results"][i]["correct_answer"]);
				questions.push_back(this->question);
			}
			return questions;
		}
	}

	std::vector<std::string> split(std::string& str, char delimiter) {
		std::vector<std::string> result;
		std::string tmp = "";

		for (int i = 0;i < str.length(); i++) {
			if (str[i] == delimiter) {
				result.push_back(tmp);
				tmp = "";
			}
			else if (i + 1 == str.length()) {
				tmp += str[i];
				result.push_back(tmp);
				tmp = "";
			}
			else {
				tmp += str[i];
			}
		}
		answers = result;
		return result;
	}
private:
	std::string KEY;

	APIManager* a_manager;

	Question question;

	int accuracy;

	std::vector<Question> questions;

	std::vector<std::string> answers;
};