#include "sre/SpriteAtlas.hpp"

#include "Engine/MyEngine.h"
#include "Engine/ComponentFactory.h"

#include "Components/ComponentCamera.h"
#include "Components/ComponentPlayer.h"
#include "Components/ComponentController.h"
#include "Components/ComponentLevelLayout.h"
#include "Components/ComponentRendererSphere.h"
#include "Components/ComponentRendererMesh.h"
#include "Engine/Components/ComponentRendererSprite.h"
#include "Components/ComponentFloorLayout.h"

MyEngine::Engine engine;

int main() {
	MyEngine::ComponentFactory::RegisterComponentOfType("CAMERA", []() { return std::make_shared<ComponentCamera>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("CONTROLLER", []() { return std::make_shared<ComponentController>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("PLAYER", []() { return std::make_shared<ComponentPlayer>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("PLAYER_RENDERER", []() { return std::make_shared<ComponentRendererSphere>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("LEVEL_LAYOUT", []() { return std::make_shared<ComponentLevelLayout>(); });
	MyEngine::ComponentFactory::RegisterComponentOfType("LEVEL_FLOOR", []() { return std::make_shared<ComponentFloorLayout>(); });
	engine.Init("data/scene.json");

}
