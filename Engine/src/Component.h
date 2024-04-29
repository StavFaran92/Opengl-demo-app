#pragma once

#include <memory>

#include "Core.h"

#include "ScriptableEntity.h"
#include "Configurations.h"
#include <glm/gtc/matrix_transform.hpp>

class Scene;
class Mesh;


struct EngineAPI Component
{

};

struct EngineAPI TagComponent
{
	[[maybe_unused]] bool empty = false;
};

struct EngineAPI SkyboxComponent : public TagComponent
{
	float t; // todo fix

	template <class Archive>
	void serialize(Archive& archive) {
		archive(t);
	}
};

struct EngineAPI RenderableComponent : public Component
{
	RenderableComponent() = default;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(renderTechnique);
	}

	enum class RenderTechnique : int
	{
		Forward,
		Deferred
	};

	RenderTechnique renderTechnique = RenderTechnique::Deferred;
};

struct EngineAPI NativeScriptComponent : public Component
{
	NativeScriptComponent() = default;
	ScriptableEntity* script = nullptr;
	Entity entity = Entity::EmptyEntity;

	template<typename T, typename... Args>
	T* bind(Args&&... args)
	{
		static_assert(std::is_base_of<ScriptableEntity, T>::value, "T must inherit from ScriptableEntity");

		script = new T(std::forward<Args>(args)...);
		script->entity = entity;
		script->onCreate();

		return static_cast<T*>(script);
	}

	void unbind()
	{
		if (script)
		{
			delete script;
			script = nullptr;
		}
	}
};

struct EngineAPI RigidBodyComponent : public Component
{
	RigidBodyComponent() = default;
	RigidBodyComponent(RigidbodyType type, float mass) : type(type), mass(mass) {};

	template <class Archive>
	void serialize(Archive& archive) {
		archive(type, mass);
	}

	RigidbodyType type = RigidbodyType::Static;
	float mass = 0;
	void* simulatedBody = nullptr;
};

struct EngineAPI CollisionBoxComponent : public Component
{
	CollisionBoxComponent() = default;
	CollisionBoxComponent(float halfExtent) : halfExtent(halfExtent) {};

	template <class Archive>
	void serialize(Archive& archive) {
		archive(halfExtent);
	}

	float halfExtent = 0;
};

struct EngineAPI CollisionSphereComponent : public Component
{
	CollisionSphereComponent() = default;
	CollisionSphereComponent(float radius) : radius(radius) {};

	template <class Archive>
	void serialize(Archive& archive) {
		archive(radius);
	}

	float radius = 0;
};

struct EngineAPI CollisionMeshComponent : public Component
{
	CollisionMeshComponent() = default;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(isConvex);
	}

	bool isConvex = false;
	Mesh* mesh = nullptr;
};

struct EngineAPI CameraComponent : public Component
{
	CameraComponent() = default;
	glm::mat4 getView()
	{
		return glm::lookAt(position, center, up);
	}

	glm::vec3 getPosition()
	{
		return position;
	}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(position, center, up);
	}

	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 up;
};

struct EngineAPI MeshComponent : public Component
{
	MeshComponent() = default;
	MeshComponent(std::shared_ptr<Mesh> mesh) : mesh(mesh) {}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(temp);
	}

	float temp; //todo remove
	std::shared_ptr<Mesh> mesh = nullptr;
};

struct EngineAPI MaterialComponent : public Component
{
	MaterialComponent() = default;
	MaterialComponent(std::shared_ptr<Material> mat) : mat(mat) {}

	std::shared_ptr<Material> mat;
};

struct EngineAPI ObjectComponent : public Component
{
	ObjectComponent() = default;
	ObjectComponent(Entity e, const std::string& name) : name(name), e(e) {};

	template <class Archive>
	void serialize(Archive& archive) {
		archive(name, e.handlerID());
	}

	std::string name;
	Entity e = Entity::EmptyEntity;
};
