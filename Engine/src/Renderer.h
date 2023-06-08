#pragma once
#include "IRenderer.h"

#include "Core.h"

// forward declerations
class ICamera;
class SkyboxRenderer;
class Model;


class EngineAPI Renderer : public IRenderer
{
public:
	// Constructor
	Renderer();

	// Copy Constructor
	Renderer(const Renderer& other);

	//Overrides
	void draw(const VertexArrayObject& vao, Shader& shader) const override;
	void render(Model* model, Shader* shader = nullptr) override;
	virtual void SetMVP(Shader& shader) const;
 	void clear() const override;
	glm::mat4 getProjection() const override;

	//Methods
	std::shared_ptr<ICamera> getCamera() const override;
	void setCamera(std::shared_ptr<ICamera> camera);
protected:
	std::shared_ptr<Shader> m_phongShader = nullptr;
	std::shared_ptr<ICamera> m_camera = nullptr;
	glm::mat4 m_projection;


};
