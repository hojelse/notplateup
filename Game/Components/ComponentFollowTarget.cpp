#include "ComponentFollowTarget.h"

#include "Engine/MyEngine.h"

void ComponentFollowTarget::Init(rapidjson::Value& serializedData) {
	Init(
		serializedData["target"].GetString(),
		glm::vec3{serializedData["offsetX"].GetFloat(), 0, 0}
	);
}
void ComponentFollowTarget::Init(
	std::string targetId,
	glm::vec3 offset
) {
	_targetId = targetId;
	_offset = offset;
	_target = MyEngine::Engine::GetInstance()->GetGameObject(_targetId);
}

void ComponentFollowTarget::Update(float deltaTime) {
	auto target = _target.lock();
	if (!target)
		return;
	
	auto gameObject = GetGameObject().lock();
	if (!gameObject)
		return;

	auto targetPos = target->GetPosition();
	auto pos = gameObject->GetPosition();

	pos = targetPos + _offset;

	gameObject->SetPosition(pos);
}