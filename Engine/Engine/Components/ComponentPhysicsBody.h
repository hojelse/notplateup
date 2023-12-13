#pragma once

#include "Engine/Component.h"

#include <Box2D/Box2D.h>
#include "Box2D/Common/b2Math.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Dynamics/b2Body.h"

class ComponentPhysicsBody : public MyEngine::Component {
	friend class MyEngine::Engine; // TODO get rid of this

public:
	~ComponentPhysicsBody();

	virtual void Init(rapidjson::Value& serializedData) override;

	void CreateBody(b2BodyType bodyType, bool isSensor, glm::vec2 size);
	// Force gradually affects the velocity over time
	void addForce(glm::vec2 force);
	// Instantly affects velocity
	void addImpulse(glm::vec2 impulse);
	void setLinearVelocity(glm::vec2 velocity);
	glm::vec2 getLinearVelocity();

private:
	b2World* _world;
	b2Body* _body = nullptr;
	b2Fixture* _fixture = nullptr;
	b2PolygonShape* _shape = nullptr;
	b2CircleShape* _shape_circle = nullptr;
	bool _is_circle = false;

	// TODO move to deserialization library (as DeserializeVector2)
	static glm::vec2 DeserializeVector2(rapidjson::Value& vectorData);
};