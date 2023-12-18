#pragma once
#include "Engine/Component.h"
#include "Engine/MyEngine.h"

class ComponentInteractable : public MyEngine::Component {
public:
	void Init(int id){
		_id = id;
	}

	virtual void Interact() {};

protected:
	int _id;
};
