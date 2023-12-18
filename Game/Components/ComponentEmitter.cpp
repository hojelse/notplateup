#include "ComponentEmitter.h"
#include "ComponentRendererMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"

void ComponentEmitter::Init(int id) {
	_id = id;
}

void ComponentEmitter::Interact() {
	auto heldVal = "itm-held";
	auto engine = MyEngine::Engine::GetInstance();
	auto isHeld = engine->GameObjectExists(heldVal);

	if (!isHeld) {
		auto item = engine->CreateGameObject(heldVal).lock();
		std::cout << "not held" << std::endl;
		auto r = std::make_shared<ComponentRendererMesh>();
		r->Init(_id);
		item->AddComponent(r);
		auto pos = glm::vec3(0, 0, 4);
		auto rot = glm::vec3(0, 0, 0);
		auto scl = glm::vec3(1, 1, 1);

		item->transform =
				glm::translate(pos) *
				glm::mat4_cast(glm::quat(glm::radians(rot))) *
				glm::scale(scl);
	} else {
		std::cout << "holding stuff" << std::endl;
	}
}
