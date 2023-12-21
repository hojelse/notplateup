#include "ComponentConsumer.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"
#include "ComponentItem.h"
#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentRendererSprite.h"

void ComponentConsumer::Init(rapidjson::Value& id) {
//	Init(id);
}

void ComponentConsumer::Update(float delta) {
	if (is_ordering) patience_left -= delta;
}

void ComponentConsumer::Init(int id) {
	_id = id;
//	CreateItemIndicator();
}

void ComponentConsumer::CreateOrder(int item_id) {
	std::cout << "Placing order " << GetGameObject().lock()->GetName() << std::endl;
	is_ordering = true;
	patience_left = 10;
	_id = item_id;
	auto engine = MyEngine::Engine::GetInstance();
	auto go = GetGameObject();
	auto parent_pos = go.lock()->GetPosition();
	auto name = "order-" + std::to_string(parent_pos.x) + "-" + std::to_string(parent_pos.y);
	auto order_icon = engine->CreateGameObject(name, go).lock();
	auto renderer = order_icon->CreateComponent<ComponentRendererSprite>().lock();
	renderer->SetSprite("sprites", "carrot.png");
	renderer->GetSprite()->setScale({0.2, 0.5});
	order_icon->SetPosition({0,0,3});
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
			is_ordering = false;
		} else {
			std::cout << "consumer and item are different types" << std::endl;
		}
	}
}

void ComponentConsumer::CreateItemIndicator() {
	std::cout << "create item indicator!" << std::endl;
	auto engine = MyEngine::Engine::GetInstance();
	auto go = GetGameObject().lock();
	auto parent_pos = go->GetPosition();

	auto name = "indicator-" + std::to_string(parent_pos.x) + "-" + std::to_string(parent_pos.y);

	auto item = engine->CreateGameObject(name).lock();
	go->AddChild(item);

	auto r = std::make_shared<ComponentRendererSprite>();
	switch (_id)
	{
		case 6: {
			r->SetSprite("sprites", "tomato.png");
			break;
		}
		case 7: {
			r->SetSprite("sprites", "carrot.png");
			break;
		}
		default: {
			r->SetSprite("sprites", "carrot.png");
			break;
		}
	}
	r->GetSprite()->setScale({0.05, 0.05});
	item->AddComponent(r);

	auto item_component = std::make_shared<ComponentItem>();
	item_component->Init(_id);
	item->AddComponent(item_component);

	auto pos = glm::vec3(parent_pos.x, parent_pos.y, 2);
	auto rot = glm::vec3(0, 0, 0);
	auto scl = glm::vec3(0.5, 0.5, 0.5);

	item->transform =
		glm::translate(pos) *
		glm::mat4_cast(glm::quat(glm::radians(rot))) *
		glm::scale(scl);
}