#include "ComponentConsumer.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"
#include "ComponentItem.h"
#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentRendererSprite.h"
#include "ComponentRendererSquare.h"

void ComponentConsumer::Update(float delta) {
	if (is_ordering) patience_left -= delta;
}

void ComponentConsumer::Init(int id) {
	_id = id;
}

void ComponentConsumer::CreateOrder(int item_id) {
	auto go = GetGameObject().lock();
	std::cout << "Placing order " << go->GetName() << std::endl;
	is_ordering = true;
	patience_left = 10;
	_id = item_id;

	auto pos = go->GetPosition();
	CreateConsumerIndicator(item_id, pos.x, pos.y);
}

void ComponentConsumer::Interact() {
	auto heldVal = "itm-held";
	auto engine = MyEngine::Engine::GetInstance();
	auto isHeld = engine->GameObjectExists(heldVal);

	if (!is_ordering) {
		std::cout << "This table is not awaiting food" << std::endl;
		return;
	}

	if (isHeld) {
		auto held = engine->GetGameObject(heldVal).lock();
		auto item = held->FindComponent<ComponentItem>().lock();
		auto item_id = item->GetTypeId();
		if (_id == item_id + 1) {
			engine->DeleteGameObject(heldVal);

			auto pos = GetGameObject().lock()->GetPosition();
			auto indicator_name = "indicator-" + std::to_string((int)pos.x) + "-" + std::to_string((int)pos.y);
			std::cout << "delete? " << indicator_name << std::endl;
			engine->DeleteGameObject(indicator_name);

			is_ordering = false;
		} else {
			std::cout << "consumer and item are different types" << std::endl;
		}
	}
}

void ComponentConsumer::CreateConsumerIndicator(int id, int x, int y) {
	auto engine = MyEngine::Engine::GetInstance();
	auto name = "indicator-" + std::to_string(x) + "-" + std::to_string(y);
	auto item = engine->CreateGameObject(name).lock();

	auto texture_name = "carrot";
	switch (id)
	{
		case 7: {
			texture_name = "tomato";
			break;
		}
		case 9: {
			texture_name = "carrot";
			break;
		}
		default: {
			texture_name = "tomato";
			break;
		}
	}

	auto r2 = std::make_shared<ComponentRendererSquare>();
	r2->Init(texture_name);
	r2->SetRotation(false, true, 0);
	item->AddComponent(r2);

	auto pos = glm::vec3(x, y, 2);
	auto rot = glm::vec3(0, 0, 0);
	auto scl = glm::vec3(0.5, 0.5, 0.5);

	item->transform =
		glm::translate(pos) *
		glm::mat4_cast(glm::quat(glm::radians(rot))) *
		glm::scale(scl);
}