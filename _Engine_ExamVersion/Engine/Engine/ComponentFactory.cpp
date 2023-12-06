#include "ComponentFactory.h"
#include "ComponentList.h"

namespace MyEngine {
	std::map<std::string, std::function<std::shared_ptr<MyEngine::Component>()>> ComponentFactory::_componentMap = {
		{ "CAMERA",[]() { return std::make_shared<ComponentCamera>(); }},
		{ "RIGIDBODY",[]() { return std::make_shared<ComponentPhysicsBody>(); }},
		{ "MESH_RENDERER",[]() { return std::make_shared<ComponentRendererMesh>(); }},
		{ "SPRITE_RENDERER",[]() { return std::make_shared<ComponentRendererSprite>(); }},
	};

	void ComponentFactory::RegisterComponentOfType(std::string typeId, std::function<std::shared_ptr<MyEngine::Component>()> builder) {
		_componentMap[typeId] = builder;
	}

	std::shared_ptr<MyEngine::Component> ComponentFactory::GetComponentOfType(std::string typeId) {
		return _componentMap[typeId]();
	}
}