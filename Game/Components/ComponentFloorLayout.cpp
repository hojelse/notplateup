#include "ComponentFloorLayout.h"

#include "ComponentRendererSquare.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/MyEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"

void ComponentFloorLayout::Init(rapidjson::Value& serializedData) {
	auto dim_y = static_cast<int>(serializedData["layout"].Size());
	auto dim_x = static_cast<int>(serializedData["layout"][0].Size());
	auto outdoor_texture_id = serializedData["outdoor_texture"].GetString();
	auto outdoor_padding = serializedData["outdoor_padding"].GetInt();

	for (int y = outdoor_padding * -1; y < dim_y + outdoor_padding; y++) {
		for (int x = outdoor_padding * -1; x < dim_x + outdoor_padding; x++) {
			if (x >= 0 && x < dim_x && y >= 0 && y < dim_y && serializedData["layout"][y][x].GetString() != "") {
				// If inside bounds
				auto texture_id = serializedData["layout"][y][x].GetString();
				CreateTile(texture_id , x, y);
			} else {
				CreateTile(outdoor_texture_id , x, y);
			}
		}
	}
}

void ComponentFloorLayout::CreateTile(std::string texture_id, int x, int y) {
	auto engine = MyEngine::Engine::GetInstance();
	auto go = engine->CreateGameObject("floor-" + std::to_string(x) + "-" + std::to_string(y)).lock();
	auto r = std::make_shared<ComponentRendererSquare>();
	r->Init(texture_id);
	r->SetRotation(true, false, 0);
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