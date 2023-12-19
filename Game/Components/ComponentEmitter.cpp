#include "ComponentEmitter.h"
#include "ComponentRendererMesh.h"
#include "Engine/Components/ComponentRendererSprite.h"
#include "ComponentFollowTarget.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"
#include "ComponentItem.h"

void ComponentEmitter::Interact() {
	auto engine = MyEngine::Engine::GetInstance();
	auto is_held = engine->GameObjectExists("itm-held");

	if (!is_held) {
		auto item = engine->CreateGameObject("itm-held").lock();

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

		auto f = std::make_shared<ComponentFollowTarget>();
		f->Init(
			"player",
			glm::vec3{0, 0, 2}
		);
		item->AddComponent(f);

		auto pos = glm::vec3(0, 0, 0);
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
