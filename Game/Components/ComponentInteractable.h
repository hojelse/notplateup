#pragma once
#include "Engine/Component.h"
#include "Engine/MyEngine.h"

class ComponentInteractable : public MyEngine::Component {
public:
	virtual void Init(std::string id){
		_id = id;
	}

	virtual void Interact() {};

protected:
	std::string _id;
};
