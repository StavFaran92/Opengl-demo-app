#pragma once

// Includes
//STL
#include <memory>

#include "glm/glm.hpp"
#include "Core.h"
#include <entt/entt.hpp>

class Shader;
class VertexArrayObject;
class Model;
class ICamera;
class Transformation;
class Mesh;
class Entity;
class Scene;
class Context;
class ICamera;

class EngineAPI IRenderer {
public:
	struct DrawQueueRenderParams
	{
		Scene* scene = nullptr;
		Context* context = nullptr;
		IRenderer* renderer = nullptr;
		entt::registry* registry = nullptr;
		

		Entity* entity = nullptr;
		Mesh* mesh = nullptr;
		Shader* shader = nullptr;

		// MVP
		glm::mat4* model = nullptr;
		glm::mat4* view = nullptr;
		glm::mat4* projection = nullptr;
	};
	enum class DrawType {
		Lines,
		Triangles
	};
public:
	IRenderer() {
		SetDrawType(DrawType::Triangles);
	};

	virtual void draw(const VertexArrayObject& vao, Shader& shader) const = 0;
	virtual void clear() const = 0;

	virtual glm::mat4 getProjection() const = 0;
	virtual void render(const DrawQueueRenderParams& renderParams) = 0;
	virtual void renderScene(DrawQueueRenderParams& renderParams) = 0;

	void SetDrawType(DrawType drawType);

protected:
	uint8_t m_drawType = 0;
};