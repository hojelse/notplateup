#include "ComponentGameLoop.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <fstream>
#include "Engine/MyEngine.h"
#include "SDL.h"
#include "ComponentFollowTarget.h"
#include "ComponentConsumer.h"
#include "rapidjson/istreamwrapper.h"
#include "ComponentLevelLayout.h"
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
	order_speedup_pr_day = serializedData["order_speedup_pr_day"].GetFloat();
	increment_order_count_day_interval = serializedData["increment_order_count_day_interval"].GetInt();
	patience_decrease_pr_day = serializedData["patience_decrease_pr_day"].GetFloat();
	table_spawn_day_interval = serializedData["table_spawn_day_interval"].GetInt();

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

void DeleteSpawnedConsumers() {
	auto engine = MyEngine::Engine::GetInstance();
	for (const auto& pair : engine->GetGameObjects()) {
		const std::string& key = pair.first;
		auto value = pair.second;

		if (key.find("box") != std::string::npos) {
			auto consumer = value->FindComponent<ComponentConsumer>().lock();
			if (consumer && !consumer->is_part_of_layout) {
				engine->DeleteGameObject(value->GetName());
				consumer->indicator = nullptr;
			}
		}
	}
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
		day++;
		SetGameState(EDIT);
		return;
	}

	time_until_next_order -= deltaTime;
	auto consumers = GetAvailableConsumers();
	if (!AllConsumersHavePatience()) {
		SetGameState(GAMEOVER);
		_death_reason = "A customer lost patience";
		std::cout << "A customer lost patience, you lost" << std::endl;
		return;
	}

	if (time_until_next_order <= 0 && orders_placed_today < orders_pr_day) {

		std::cout << "Consumer size: " << std::to_string(consumers.size()) << std::endl;
		if (consumers.size() == 0) {
			SetGameState(GAMEOVER);
			_death_reason = "No available table";
			std::cout << "No available tables, you lost" << std::endl;
			return;
		}
		std::srand(std::time(nullptr));
		auto item_id = std::rand() % items.size();
		auto consumer_index = std::rand() % consumers.size();
		orders_placed_today++;
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
	DeleteSpawnedConsumers();
	time_between_orders = initial_time_between_orders;
	customer_patience = initial_customer_patience;
	orders_pr_day = initial_orders_pr_day;
	day = 0;
	_death_reason = "";
	// TODO clear all gameobject and create level from scratch
}

void ComponentGameLoop::ClearDay() {
	ResetConsumers();
	DeleteItems();
	time_until_next_order = time_between_orders;
	orders_placed_today = 0;
	orders_completed_today = 0;
	time_between_orders = initial_time_between_orders * std::pow(order_speedup_pr_day, day);
	orders_pr_day = initial_orders_pr_day + std::floor(day/increment_order_count_day_interval);
	customer_patience = initial_customer_patience * std::pow(patience_decrease_pr_day, day);
	if (day > 0 && day % table_spawn_day_interval == 0) {
		auto level_layout = MyEngine::Engine::GetInstance()->GetGameObject("layout").lock()->FindComponent<ComponentLevelLayout>().lock();
		level_layout->CreateBox("consumer", 0, -1, false);
	}
}

void ComponentGameLoop::KeyEvent(SDL_Event &event) {
	switch (event.key.keysym.sym) {
		case SDLK_SPACE:
			auto box_held = MyEngine::Engine::GetInstance()->GameObjectExists("box-held");
			if (event.type == SDL_KEYDOWN) {
				switch (_game_state) {
					case EDIT:
						if (!box_held) SetGameState(PLAYING);
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

void ComponentGameLoop::Render(sre::RenderPass &) {
	ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiSetCond_Always);
	if (day == 0 && _game_state == EDIT) {
		ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_Always);
	} else {
		ImGui::SetNextWindowSize(ImVec2(300, 75), ImGuiSetCond_Always);
	}
	ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	if (day == 0 && _game_state == EDIT) {
		ImGui::Text("%s", "Welcome To !PlateUp");
		ImGui::TextWrapped("%s", "Your task as chef is to serve your customers what they order, before they run out of patience.");
		ImGui::TextWrapped("%s", "Also make sure that there is always a table available for the next customer, otherwise you will lose.");
		ImGui::TextWrapped("%s", "Before each day begins, you can arrange your restaurant as you desire. Each day will be a little harder than the one before.");
		ImGui::TextWrapped("%s", "");
		ImGui::TextWrapped("%s", "Controls:");
		ImGui::TextWrapped("%s", "Movement: W A S D");
		ImGui::TextWrapped("%s", "Interact: O");
		ImGui::TextWrapped("%s", "Dash: K");
		ImGui::TextWrapped("%s", "Look without walking: LSHIFT");
		ImGui::TextWrapped("%s", "");
	}

	std::string state_text = "";
	std::string action_text = "";
	switch (_game_state) {
		case EDIT:
			state_text = "Prepare your restaurant";
			action_text = "Press SPACE to start the next day";
			break;
		case PLAYING:
			state_text = "Serve your customers";
			action_text = std::to_string(orders_completed_today) + " out of " + std::to_string(orders_pr_day) + " orders completed";
			break;
		case GAMEOVER:
			state_text = "You lost. " + _death_reason;
			action_text = "Press SPACE to start over";
			break;
	}
	ImGui::Text("Day %i", day);
	ImGui::Text("%s", state_text.c_str());
	ImGui::Text("%s", action_text.c_str());
	ImGui::End();
}

void ComponentGameLoop::OrderCompleted() {
	orders_completed_today++;
}