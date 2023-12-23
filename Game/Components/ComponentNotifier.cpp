#include "ComponentNotifier.h"
#include "Engine/MyEngine.h"

void ComponentNotifier::Init(rapidjson::Value &serializedData) {
	_total_duration = serializedData["total_duration"].GetFloat();
	_duration_remaining = 0;
	_message = "";
}

void ComponentNotifier::Update(float delta) {
	if (_duration_remaining > 0) _duration_remaining -= delta;
	else _message = "";
}

void ComponentNotifier::Render(sre::RenderPass &) {
	if (_message.empty()) return;
	ImGui::SetNextWindowPos(ImVec2(500, .0f), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, 50), ImGuiSetCond_Always);
	ImGui::Begin("notifier", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::TextWrapped("%s", _message.c_str());
	ImGui::End();
}

void ComponentNotifier::SetMessage(std::string message) {
	_message = message;
	_duration_remaining = _total_duration;
}