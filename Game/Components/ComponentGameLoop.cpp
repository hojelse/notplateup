#include "ComponentGameLoop.h"
#include <glm/gtx/matrix_decompose.hpp>
#include "Engine/MyEngine.h"
#include "SDL.h"
#include "ComponentFollowTarget.h"
#include "ComponentConsumer.h"

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

void ComponentGameLoop::Init(rapidjson::Value &serializedData) {
	time_between_orders = serializedData["order_interval"].GetFloat();
	customer_patience = serializedData["customer_patience"].GetFloat();
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

		consumers[0]->CreateOrder("item-tomato", customer_patience);
		time_until_next_order = time_between_orders;
	}
}

void ComponentGameLoop::KeyEvent(SDL_Event &event) {

}
