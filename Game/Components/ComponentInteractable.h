#pragma once
#include "Engine/Component.h"
#include "Engine/MyEngine.h"

class ComponentInteractable : public MyEngine::Component {
public:
	virtual void Init(std::string id){
		_id = id;
	}

	virtual void Interact() {};

	std::string GeneralId() {
		size_t dashPosition = _id.find('-');

		if (dashPosition != std::string::npos) {
			return _id.substr(dashPosition + 1);
		}
		return _id;
	}

protected:
	std::string _id;
};
