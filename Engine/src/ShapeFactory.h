#pragma once

#include <memory>

#include "Core.h"

class Entity;

class EngineAPI ShapeFactory
{
public:
	static std::shared_ptr<Entity> createBox();
	static std::shared_ptr<Entity> createPlane();
	static std::shared_ptr<Entity> createSphere(float radius, int sectors, int stacks);

private:
	static std::shared_ptr<Entity> createEntity();
};