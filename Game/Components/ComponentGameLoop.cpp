#include "ComponentGameLoop.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <fstream>
#include "Engine/MyEngine.h"
#include "SDL.h"
#include "ComponentFollowTarget.h"
#include "ComponentConsumer.h"
#include "rapidjson/istreamwrapper.h"
#include <ctime>

std::vector<std::shared_ptr<ComponentConsumer>> GetAvailableConsumers() {
	auto engine = MyEngine::Engine::GetInstance();
	std::vector<std::shared_ptr<ComponentConsumer>> consumers = {};
	for (const auto& pair : engine->GetGameObjects()) {
		const std::string& key = pair.first;
		auto value = pair.second;

		if (key.find("box") != std::string::npos) {
			auto consumer = value->FindComponent<ComponentConsumer>().lock();
			if (consumer) {
				if (!consumer->is_ordering) consumers.push_back(consumer);
			}
		}
	}

	return consumers;
}

bool startsWith(const std::string& fullString, const std::string& prefix) {
	return fullString.compare(0, prefix.length(), prefix) == 0;
}

GameState ComponentGameLoop::GetGameState() {
	return _game_state;
}

void ComponentGameLoop::Init(rapidjson::Value &serializedData) {
	initial_time_between_orders = serializedData["initial_time_between_orders"].GetFloat();
	initial_customer_patience = serializedData["initial_customer_patience"].GetFloat();
	initial_orders_pr_day = serializedData["initial_orders_pr_day"].GetInt();

	time_between_orders = initial_time_between_orders;
	customer_patience = initial_customer_patience;
	orders_pr_day = initial_orders_pr_day;

	std::ifstream fis("data/sprites.json");
	rapidjson::IStreamWrapper isw(fis);
	rapidjson::Document document;
	document.ParseStream(isw);

	auto frames = document["frames"].GetArray();
	int i = 0;
	for (; i < frames.Size(); i++) {
		std::string frame_name = frames[i]["filename"].GetString();
		if (startsWith(frame_name, "item-")) {
			auto without_extension = frame_name.substr(0, frame_name.length() - 4);
			items.emplace_back(without_extension);
		}
	}
}

bool AllConsumersHavePatience() {
	auto engine = MyEngine::Engine::GetInstance();
	for (const auto& pair : engine->GetGameObjects()) {
		const std::string& key = pair.first;
		auto value = pair.second;

		if (key.find("box") != std::string::npos) {
			auto consumer = value->FindComponent<ComponentConsumer>().lock();
			if (consumer && consumer->patience_left <= 0 && consumer->is_ordering) {
				return false;
			}
		}
	}

	return true;
}

void ResetConsumers() {
	auto engine = MyEngine::Engine::GetInstance();
	for (const auto& pair : engine->GetGameObjects()) {
		const std::string& key = pair.first;
		auto value = pair.second;

		if (key.find("box") != std::string::npos) {
			auto consumer = value->FindComponent<ComponentConsumer>().lock();
			if (consumer) {
				consumer->is_ordering = false;
				if (consumer->indicator) {
					engine->DeleteGameObject(consumer->indicator->GetName());
					consumer->indicator = nullptr;
				}
			}
		}
	}
}

void ComponentGameLoop::Update(float deltaTime) {
	if (_game_state != PLAYING) return;

	if (orders_completed_today >= orders_pr_day) {
		SetGameState(EDIT);
		return;
	}

	time_until_next_order -= deltaTime;
	auto consumers = GetAvailableConsumers();
	if (!AllConsumersHavePatience()) {
		SetGameState(GAMEOVER);
		std::cout << "A customer lost patience, you lost" << std::endl;
		return;
	}

	if (time_until_next_order <= 0) {

		std::cout << "Consumer size: " << std::to_string(consumers.size()) << std::endl;
		if (consumers.size() == 0) {
			SetGameState(GAMEOVER);
			std::cout << "No available tables, you lost" << std::endl;
			return;
		}
		std::srand(std::time(nullptr));
		auto item_id = std::rand() % items.size();
		auto consumer_index = std::rand() % consumers.size();
		consumers[consumer_index]->CreateOrder(items[item_id], customer_patience);

		time_until_next_order = time_between_orders;
	}
}

void ComponentGameLoop::SetGameState(GameState new_state) {
	if (new_state == GAMEOVER) ResetGame();
	if (new_state == EDIT) ClearDay();

	std::cout << "Game state set to " << std::to_string(new_state) << std::endl;
	_game_state = new_state;
}

void DeleteItems() {
	auto engine = MyEngine::Engine::GetInstance();
	for (const auto& pair : engine->GetGameObjects()) {
		const std::string& key = pair.first;
		auto value = pair.second;

		if (key.find("itm-") != std::string::npos) {
			engine->DeleteGameObject(value->GetName());
		}
	}
}

void ComponentGameLoop::ResetGame() {
	ClearDay();
	orders_pr_day = initial_orders_pr_day;
	orders_completed_today = 0;
	// TODO clear all gameobject and create level from scratch
}

void ComponentGameLoop::ClearDay() {
	ResetConsumers();
	DeleteItems();
	time_until_next_order = time_between_orders;
}

void ComponentGameLoop::KeyEvent(SDL_Event &event) {
	switch (event.key.keysym.sym) {
		case SDLK_SPACE:
			if (event.type == SDL_KEYDOWN) {
				switch (_game_state) {
					case EDIT:
						SetGameState(PLAYING);
						break;
					case GAMEOVER:
						SetGameState(EDIT);
						break;
					default:
						break;
				}
			}
			break;
	}
}

void ComponentGameLoop::OrderCompleted() {
	orders_completed_today++;
}