#include "ComponentLevelLayout.h"
#include "ComponentRendererMesh.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/MyEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"

void ComponentLevelLayout::Init(rapidjson::Value& serializedData) {
	auto dimy = serializedData["layout"].Size();
	auto dimx = serializedData["layout"][0].Size();
	
	auto engine = MyEngine::Engine::GetInstance();

	for (int y = 0; y < dimy; y++) {
		for (int x = 0; x < dimx; x++) {
			auto texture_id = serializedData["layout"][y][x].GetInt();
			if (texture_id < 0) continue;
			if (texture_id > 6*16-1) continue;

			auto go = engine->CreateGameObject("box-" + std::to_string(x) + "-" + std::to_string(y)).lock();

			auto r = std::make_shared<ComponentRendererMesh>();
			r->Init(serializedData["layout"][y][x]);
			go->AddComponent(r);

			auto pos = glm::vec3(x, dimy-y, 0);
			auto rot = glm::vec3(0, 0, 0);
			auto scl = glm::vec3(1, 1, 1);

			go->transform =
				glm::translate(pos) *
				glm::mat4_cast(glm::quat(glm::radians(rot))) * 
				glm::scale(scl);

			auto body = go->CreateComponent<ComponentPhysicsBody>().lock();

			auto bodyType = b2_staticBody;
			auto isSensor = false;
			glm::vec2 size { 0.5, 0.5 };
			body->CreateBody(bodyType, isSensor, size);
		}
	}
}

void ComponentLevelLayout::Update(float deltaTime) {

}

void ComponentLevelLayout::Render(sre::RenderPass& renderPass) {

}