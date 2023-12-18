#include "ComponentEmitter.h"
#include "ComponentRendererMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"
#include "ComponentItem.h"

void ComponentEmitter::Interact() {
	auto engine = MyEngine::Engine::GetInstance();
	auto is_held = engine->GameObjectExists("itm-held");

	if (!is_held) {
		auto item = engine->CreateGameObject("itm-held").lock();
		auto r = std::make_shared<ComponentRendererMesh>();
		r->Init(_id);
		item->AddComponent(r);

		auto item_component = std::make_shared<ComponentItem>();
		item_component->Init(_id);
		item->AddComponent(item_component);

		auto pos = glm::vec3(0, 0, 4);
		auto rot = glm::vec3(0, 0, 0);
		auto scl = glm::vec3(0.5, 0.5, 0.5);

		item->transform =
				glm::translate(pos) *
				glm::mat4_cast(glm::quat(glm::radians(rot))) *
				glm::scale(scl);
	} else {
		std::cout << "holding stuff" << std::endl;
	}
}
