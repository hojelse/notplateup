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
	for (const auto& pair : engine->_gameObjects) {
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

void ComponentGameLoop::Init(rapidjson::Value &serializedData) {
	time_between_orders = serializedData["order_interval"].GetFloat();
	customer_patience = serializedData["customer_patience"].GetFloat();
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
	for (const auto& pair : engine->_gameObjects) {
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

void ComponentGameLoop::Update(float deltaTime) {
	time_until_next_order -= deltaTime;
	auto consumers = GetAvailableConsumers();
	if (!AllConsumersHavePatience()) {
		std::cout << "A customer lost patience, you lost" << std::endl;
		return;
	}
	if (time_until_next_order <= 0) {

		std::cout << "Consumer size: " << std::to_string(consumers.size()) << std::endl;
		if (consumers.size() == 0) {
			std::cout << "No available tables, you lost" << std::endl;
			return;
		}
		std::srand(std::time(nullptr));
		auto item_id = std::rand() % items.size();
		consumers[0]->CreateOrder(items[item_id], customer_patience);
		time_until_next_order = time_between_orders;
	}
}

void ComponentGameLoop::KeyEvent(SDL_Event &event) {

}
