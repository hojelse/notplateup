#include "ComponentTable.h"
#include "ComponentFollowTarget.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ComponentItem.h"

void ComponentTable::Interact() {
	auto engine = MyEngine::Engine::GetInstance();
	auto is_holding_item = engine->GameObjectExists("itm-held");

	auto go = GetGameObject().lock();
	auto table_pos = go->GetPosition();
	auto item_on_box_id = "itm-" + std::to_string(table_pos.x) + "-" + std::to_string(table_pos.y);
	auto has_item_on_box = engine->GameObjectExists(item_on_box_id);

	if (is_holding_item && !has_item_on_box) {

		auto held_item = engine->GetGameObject("itm-held").lock();
		held_item->SetPosition(table_pos + glm::vec3(0,0,1));

		engine->RenameGameObject("itm-held", item_on_box_id);
		
		auto f = held_item->FindComponent<ComponentFollowTarget>().lock();
		held_item->RemoveComponent(f);

	} else if (!is_holding_item && has_item_on_box) {

		auto item_on_box = engine->GetGameObject(item_on_box_id).lock();

		item_on_box->SetPosition({0, 0, -2});
		engine->RenameGameObject(item_on_box_id, "itm-held");

		auto f = std::make_shared<ComponentFollowTarget>();
		f->Init(
			"player",
			glm::vec3{0, 0, 2}
		);
		item_on_box->AddComponent(f);
	}
}
