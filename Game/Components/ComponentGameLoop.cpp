#include "ComponentGameLoop.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Engine/MyEngine.h"

#include "SDL.h"
#include "ComponentInteractable.h"
#include "ComponentFollowTarget.h"
#include "ComponentConsumer.h"
#include <cmath>

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
//	auto dim = serializedData["orders"].Size();
//	_n = dim/2;
//
//	auto engine = MyEngine::Engine::GetInstance();
//
//	for (int i = 0; i < dim; i += 2) {
//		_start.push_back(serializedData["orders"][i].GetFloat());
//		_item_id.push_back(serializedData["orders"][i+1].GetInt());
//	}
//
//	// state
//	// 0 -> future
//	// 1 -> active
//	// 2 -> inactive
//	_item_states = std::vector<int>(_n, 0);
//
//	// game state
//	// 0 -> Paused
//	// 1 -> Active
//	// 2 -> Lost
//	_game_state = 1;
//
//	_patience = 15;
//	_elapsed = 0.0f;
}

bool AllConsumersHavePatience(std::vector<std::shared_ptr<ComponentConsumer>> consumers) {
	for (auto& consumer : consumers) {
		if (consumer->patience_left <= 0) return false;
	}
	return true;
}

void ComponentGameLoop::Update(float deltaTime) {
	time_until_next_order -= deltaTime;
	if (time_until_next_order <= 0) {
		auto consumers = GetAvailableConsumers();

		std::cout << "Consumer size: " << std::to_string(consumers.size()) << std::endl;
		if (consumers.size() == 0) {
			std::cout << "No available tables, you lost" << std::endl;
			return;
		}

		if (!AllConsumersHavePatience(consumers)) {
			std::cout << "A customer lost patience, you lost" << std::endl;
		}

		std::cout << "Placing order " << consumers[0]->GetGameObject().lock()->GetName() << std::endl;
		consumers[0]->is_ordering = true;
		consumers[0]->patience_left = 10;
		consumers[0]->Init(7);
		time_until_next_order = time_between_orders;
	}


//	_elapsed += deltaTime;
//
//	switch (_game_state)
//	{
//		case 0:
//			break;
//		case 1: {
//			for (int i = 0; i < _n; i++) {
//				if (_item_states[i] == 0 && _start[i] < _elapsed && _elapsed < _start[i] + _patience) {
//					_item_states[i] = 1;
//				}
//			}
//
//			for (int i = 0; i < _n; i++) {
//				if (_item_states[i] == 1) {
//					std::cout << i << " " << (_item_id[i] == 6 ? "tomato" : "carrot") << " in " << ((_start[i] + _patience) - _elapsed) << "sec" << std::endl;
//
//					if (_elapsed > _start[i] + _patience) {
//						std::cout << "You lost!" << std::endl;
//						_item_states[i] = 2;
//						_game_state = 2;
//					}
//				}
//			}
//			break;
//		}
//		case 2: {
//			std::cout << "You lost!" << std::endl;
//			break;
//		}
//		default:
//			break;
//	}

}

//void ComponentGameLoop::SubmitItem(int idx) {
//	_item_states[idx] = 2;
//}

void ComponentGameLoop::KeyEvent(SDL_Event &event) {
//	switch (event.key.keysym.sym) {
//		case SDLK_i:
//			key_down_i = event.type == SDL_KEYDOWN;
//			break;
//	}
}
