#include "ComponentFloorLayout.h"

#include "ComponentRendererSquare.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/MyEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"
#include "ComponentEmitter.h"
#include "ComponentConsumer.h"
#include "ComponentTable.h"

void ComponentFloorLayout::Init(rapidjson::Value& serializedData) {
	auto dimy = serializedData["layout"].Size();
	auto dimx = serializedData["layout"][0].Size();

	auto engine = MyEngine::Engine::GetInstance();

	for (int y = 0; y < dimy; y++) {
		for (int x = 0; x < dimx; x++) {
			auto texture_id = serializedData["layout"][y][x].GetInt();

			if (texture_id < 0) continue;
			if (texture_id > 6*16-1) continue;
			CreateBox(texture_id, x, y);
		}
	}
}

void ComponentFloorLayout::CreateBox(int texture_id, int x, int y) {
	auto engine = MyEngine::Engine::GetInstance();
	auto go = engine->CreateGameObject("floor-" + std::to_string(x) + "-" + std::to_string(y)).lock();
	auto r = std::make_shared<ComponentRendererSquare>();
	r->Init(texture_id);
	go->AddComponent(r);

	auto pos = glm::vec3(x, y, -1);
	auto rot = glm::vec3(0, 0, 0);
	auto scl = glm::vec3(1, 1, 1);

	go->transform =
			glm::translate(pos) *
			glm::mat4_cast(glm::quat(glm::radians(rot))) *
			glm::scale(scl);
}

void ComponentFloorLayout::Update(float deltaTime) {

}

void ComponentFloorLayout::Render(sre::RenderPass& renderPass) {

}