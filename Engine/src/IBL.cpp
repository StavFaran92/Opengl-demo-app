#include "IBL.h"

#include "Texture.h"
#include "FrameBufferObject.h"
#include "RenderBufferObject.h"
#include "TextureHandler.h"
#include "Shader.h"
#include "ShapeFactory.h"

#include "GL/glew.h"
#include "glm/ext.hpp"
#include "Logger.h"

#include "RenderCommand.h"
#include "Entity.h"
#include "Component.h"
#include "Mesh.h"

TextureHandler* IBL::generateIrradianceMap(TextureHandler* environmentMap, Scene* scene)
{
	auto irradianceShader = Shader::create<Shader>(
		"Resources/Engine/Shaders/IrradianceShader.vert",
		"Resources/Engine/Shaders/IrradianceShader.frag");

	// Generate FBO 
	FrameBufferObject fbo;

	fbo.bind();

	// Generate cubemap
	auto irradianceMap = Texture::createCubemapTexture(32, 32, GL_RGB16F, GL_RGB, GL_FLOAT);

	RenderBufferObject rbo{ 32, 32 };
	fbo.attachRenderBuffer(rbo.GetID(), FrameBufferObject::AttachmentType::Depth);

	if (!fbo.isComplete())
	{
		logError("FBO is not complete!");
		return nullptr;
	}

	// Generate views and projection
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// set viewport
	glViewport(0, 0, 32, 32);

	irradianceShader->use();
	irradianceShader->setProjectionMatrix(captureProjection);
	irradianceShader->setValue("environmentMap", 0);

	environmentMap->setSlot(0);
	environmentMap->bind();


	auto box = ShapeFactory::createBox(scene);
	auto vao = box.getComponent<MeshComponent>().mesh->getVAO();

	// render to cube
	// Attach cube map to frame buffer
	for (int i = 0; i < 6; i++)
	{
		// set view
		irradianceShader->setViewMatrix(captureViews[i]);

		// attach cubemap face to fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap->getID(), 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render cube
		RenderCommand::drawIndexed(vao);
	}

	environmentMap->unbind();
	fbo.unbind();

	return irradianceMap;
}
