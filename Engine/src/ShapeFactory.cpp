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
//#include "RenderableComponent.h"
#include "Component.h"

Entity ShapeFactory::createEntity(Scene* scene)
{
	auto entity = scene->createEntity();
	entity.addComponent<DefaultMaterial>(32.0f);
	auto shader = Shader::create<StandardShader>();
	entity.addComponentInst<StandardShader>(shader);
	entity.addComponent<RenderableComponent>();
	return entity;
}

Entity ShapeFactory::createBox(Scene* scene)
{
	auto entity = createEntity(scene);
	auto mesh = Box::createMesh();
	entity.addComponentInst<Mesh>(mesh);
	return entity;
}

Entity ShapeFactory::createQuad(Scene* scene)
{
	auto entity = createEntity(scene);
	auto mesh = Quad::createMesh();
	entity.addComponentInst<Mesh>(mesh);
	return entity;
}

Entity ShapeFactory::createSphere(Scene* scene, float radius, int sectors, int stacks)
{
	auto entity = createEntity(scene);
	auto mesh = Sphere::createMesh(radius, sectors, stacks);
	entity.addComponentInst<Mesh>(mesh);
	return entity;
}
