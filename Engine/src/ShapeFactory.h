#pragma once

#include <memory>
#include <string>

#include "Core.h"

class Entity;
class Scene;

class EngineAPI ShapeFactory
{
public:
	static Entity createBox(Scene* scene);
	static Entity createQuad(Scene* scene);
	static Entity createSphere(Scene* scene);

private:
	static Entity createEntity(Scene* scene, const std::string& name);
};