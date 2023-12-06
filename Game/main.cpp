#include "sre/SpriteAtlas.hpp"

#include "Engine/MyEngine.h"
#include "Engine/ComponentFactory.h"

#include "Components/ComponentCamera.h"
#include "Components/ComponentPlayer.h"
#include "Components/ComponentController.h"
#include "Components/ComponentLevelLayout.h"
#include "Components/ComponentRendererSphere.h"

MyEngine::Engine engine;

int main() {
	MyEngine::ComponentFactory::RegisterComponentOfType("CAMERA", std::make_shared<ComponentCamera>());
	MyEngine::ComponentFactory::RegisterComponentOfType("CONTROLLER", std::make_shared<ComponentController>());
	MyEngine::ComponentFactory::RegisterComponentOfType("PLAYER", std::make_shared<ComponentPlayer>());
	MyEngine::ComponentFactory::RegisterComponentOfType("PLAYER_RENDERER", std::make_shared<ComponentRendererSphere>());
	MyEngine::ComponentFactory::RegisterComponentOfType("LEVEL_LAYOUT", std::make_shared<ComponentLevelLayout>());
	engine.Init("data/scene.json");

}
