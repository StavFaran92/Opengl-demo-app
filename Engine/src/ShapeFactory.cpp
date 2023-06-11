#include "ShapeFactory.h"

#include "Engine.h"
#include "Context.h"
#include "Mesh.h"
#include "Entity.h"
#include "DefaultMaterial.h"
#include "Box.h"
#include "Quad.h"
#include "Sphere.h"
#include "Scene.h"
#include "Transformation.h"
#include "StandardShader.h"
#include "RenderableComponent.h"

std::shared_ptr<Entity> ShapeFactory::createEntity(Scene* scene)
{
	auto entity = scene->createEntity();
	entity->addComponent<DefaultMaterial>(32.0f);
	entity->addComponent<Transformation>();
	auto shader = Shader::create<StandardShader>();
	entity->addComponent<StandardShader>(shader);
	entity->addComponent<Renderable>();
	return entity;
}

std::shared_ptr<Entity> ShapeFactory::createBox(Scene* scene)
{
	auto entity = createEntity(scene);
	auto mesh = Box::createMesh();
	entity->addComponent<Mesh>(mesh);
	return entity;
}

std::shared_ptr<Entity> ShapeFactory::createPlane(Scene* scene)
{
	auto entity = createEntity(scene);
	auto mesh = Quad::createMesh();
	entity->addComponent<Mesh>(mesh);
	return entity;
}

std::shared_ptr<Entity> ShapeFactory::createSphere(Scene* scene, float radius, int sectors, int stacks)
{
	auto entity = createEntity(scene);
	auto mesh = Sphere::createMesh(radius, sectors, stacks);
	entity->addComponent<Mesh>(mesh);
	return entity;
}
