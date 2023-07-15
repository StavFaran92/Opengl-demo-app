#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core.h"

#include "Entity.h"

class EngineAPI Transformation
{
public:
	Transformation(const Entity& entity) :
		m_entity(entity),
		m_translation(0, 0, 0),
		m_rotation(1, 0, 0, 0),
		m_scale(1, 1, 1),
		m_relativeRot(1.f)
	{}

	Transformation(const Entity& entity, glm::vec3 translation) :
		m_entity(entity),
		m_translation(translation),
		m_rotation(1, 0, 0, 0),
		m_scale(1, 1, 1),
		m_relativeRot(1.f)
	{
		m_change = true;
	}

	Transformation(const Entity& entity, glm::vec3 translation, glm::quat rotation) :
		m_entity(entity),
		m_translation(translation),
		m_rotation(rotation),
		m_scale(1, 1, 1),
		m_relativeRot(1.f)
	{
		m_change = true;
	}


	void setLocalPosition(glm::vec3 pos);
	void setWorldPosition(glm::vec3 pos);
	void setLocalRotation(float angle, glm::vec3 axis);
	void setLocalRotation(glm::quat quat);
	void setLocalScale(glm::vec3 scale);

	glm::mat4 getWorldTransformation();
	glm::vec3 getLocalPosition() const;
	glm::vec3 getWorldPosition() const;
	glm::quat getLocalRotation() const;
	glm::quat getWorldRotation() const;
	glm::vec3 getLocalScale() const;
	glm::vec3 getWorldScale() const;

	void translate(float x, float y, float z);
	void translate(glm::vec3 translation);

	void scale(float x, float y, float z);
	void scale(glm::vec3 translation);

	void rotate(glm::vec3 eulers);
	void rotate(glm::vec3 axis, float angle);
	void rotateAround(glm::vec3 pivot, glm::vec3 axis, float angle);
	void rotateLerp(glm::vec3 axis, float angle, float t);
	void rotateAroundLerp(glm::vec3 pivot, glm::vec3 axis, float angle, float t);
	void getWorldTransformation(glm::mat4& mat);
	glm::mat4 getLocalTransformation();

	void setParent(Entity parent);
	void removeParent();
	Entity getParent() const;

	std::unordered_map<entity_id, Entity> getChildren();

private:
	void addChild(Entity entity);
	void removeChild(Entity entity);
private:
	glm::vec3 m_translation;

	glm::quat m_rotation;

	glm::mat4 m_relativeRot;

	glm::vec3 m_scale;

	glm::mat4 m_rootTransformation{ 1.f };

	Entity m_parent = Entity::EmptyEntity;
	Entity m_entity = Entity::EmptyEntity;
	std::unordered_map<entity_id, Entity> m_children{};

	bool m_change = false;
};
