#include "ComponentCamera.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "rapidjson/document.h"
#include "Engine/MyEngine.h"

void ComponentCamera::Init(rapidjson::Value& serializedData) {
	_camera = MyEngine::Engine::GetInstance()->GetCamera();

	auto& projectionParams = serializedData["projectionParams"];
	// this is EXTREMELY unsafe. It would be fine if we both
	//   - ensure that the json is always read and write by code, and never touched by humans
	//   - hide all of this where client programmers don't need to deal with it
	// which is not our case. But for the sake of simplicity, we will accept it
	float p_0 = projectionParams[0].GetFloat();
	float p_1 = projectionParams[1].GetFloat();
	float p_2 = projectionParams[2].GetFloat();

	switch (serializedData["projectionType"].GetInt()) {
		case ProjectionType::Orthographic: _camera->setOrthographicProjection(p_0, p_1, p_2); break;
		case ProjectionType::Perspective:  _camera->setPerspectiveProjection(p_0, p_1, p_2);  break;
	}

	_camera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3{ 0,1,0 });
}

void ComponentCamera::Update(float delta) {
	if (_debugging)
		if (auto gameObject = GetGameObject().lock()) {
			glm::vec3 pos = gameObject->GetPosition();
			pos.x += _debugMov.x * _debugMovSpeed;
			pos.y += _debugMov.y * _debugMovSpeed;
			gameObject->SetPosition(pos);
		}

	if (auto gameObject = GetGameObject().lock())
	{
		glm::vec3 position = gameObject->GetPosition();
		glm::quat rotation = gameObject->GetRotation();
		_camera->setPositionAndRotation(position, glm::degrees(glm::eulerAngles(rotation)));
	}

}

void ComponentCamera::KeyEvent(SDL_Event& event) {
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F3)
	{
		_debugging = !_debugging;
		return;
	}

	if (!_debugging)
		return;

	switch (event.key.keysym.sym) {
		case SDLK_w: _debugMov.y = -(event.type == SDL_KEYDOWN); break;
		case SDLK_s: _debugMov.y = +(event.type == SDL_KEYDOWN); break;
		case SDLK_a: _debugMov.x = +(event.type == SDL_KEYDOWN); break;
		case SDLK_d: _debugMov.x = -(event.type == SDL_KEYDOWN); break;
	}
}