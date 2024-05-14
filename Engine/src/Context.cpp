#include "Context.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "Skybox.h"
#include "SkyboxRenderer.h"
#include "ICamera.h"
#include "PhongShader.h"
#include "PickingShader.h"
#include "Shader.h"

#include "Object3D.h"
#include "Scene.h"
#include "Engine.h"

#include "Logger.h"
#include "Engine.h"
#include "Texture.h"
#include "Material.h"
#include "ProjectAssetRegistry.h"

Context::Context() : m_shaderCounter(0)
{
	m_projectAssetRegistry = ProjectAssetRegistry::create("Content/tmp2.json");

	init();
}

Context::Context(const std::string& filepath)
{
	m_projectAssetRegistry = ProjectAssetRegistry::parse("Content/tmp.json");

	init();
}

void Context::init()
{
	m_reflectionShader = Shader::createShared<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/ReflectionShader.glsl");

	m_refractiveShader = Shader::createShared<Shader>(SGE_ROOT_DIR + "Resources/Engine/Shaders/RefractionShader.glsl");


	m_pickingShader = Shader::createShared<PickingShader>();

	m_dummyTexture = Texture::createDummyTexture();

	m_defaultMaterial = std::make_shared<Material>();
}

bool Context::addScene(std::shared_ptr<Scene> scene)
{
	m_scenesCounter += 1;
	scene->SetID(m_scenesCounter);
	m_scenes.emplace(m_scenesCounter, scene);

	logInfo("Scene {} Added successfully.", std::to_string(m_scenesCounter));

	return true;
}

bool Context::removeScene(std::shared_ptr<Scene> scene)
{
	return false;
}

bool Context::AddShader(std::shared_ptr<Shader> shader)
{
	m_shaderCounter += 1;
	shader->SetID(m_shaderCounter);
	m_shaders.emplace(m_shaderCounter, shader);

	logInfo("Shader {} Added successfully.", std::to_string(m_shaderCounter));

	return true;
}

bool Context::RemoveShader(std::shared_ptr<Shader> shader)
{
	uint32_t uid = shader->getID();
	auto iter = m_shaders.find(uid);
	if (iter == m_shaders.end())
	{
		logError("Could not locate shader {}", uid);
		return false;
	}

	m_shaders.erase(iter);

	logInfo("Shader {} Erased successfully.", std::to_string(uid));

	return true;
}

Shader* Context::GetReflectionShader() const
{
	return m_reflectionShader.get();
}

Shader* Context::GetRefractiveShader() const
{
	return m_refractiveShader.get();
}

Shader* Context::getStandardShader() const
{
	return m_phongShader.get();
}

PickingShader* Context::getPickingShader() const
{
	return m_pickingShader.get();
}

std::shared_ptr<Scene> Context::getActiveScene() const
{
	if (m_activeScene == -1)
		return nullptr;

	return m_scenes.at(m_activeScene);
}

ProjectAssetRegistry* Context::getProjectAssetRegistry() const
{
	return m_projectAssetRegistry.get();
}

void Context::populateScenesFromJSON(const std::string& json)
{
	auto defaultScene = std::make_shared<Scene>(this);

	addScene(defaultScene);
	m_activeScene = defaultScene->getID();

	defaultScene->deserialize();
}

void Context::setActiveScene(uint32_t index)
{
	if (index > m_scenesCounter)
	{
		logError("Illegal index specified: " + index);
		return;
	}

	m_activeScene = index;
}

Resource<Texture> Context::getDummyTexture()
{
	return m_dummyTexture;
}



void Context::update(float deltaTime)
{
	if (m_activeScene == -1)
		return;

	if (!m_scenes[m_activeScene]->isSimulationActive())
	{
		m_scenes[m_activeScene]->startSimulation();
	}

	m_scenes[m_activeScene]->update(deltaTime);
}

void Context::draw(float deltaTime)
{
	if (m_activeScene == -1)
		return;

	m_scenes[m_activeScene]->draw(deltaTime);
}

Shader* Context::getNormalDisplayShader() const
{
	return m_normalDisplayShader.get();
}

Window* Context::getWindow() const
{
	return Engine::get()->getWindow();
}

IRenderer* Context::getRenderer() const
{
	return Engine::get()->getRenderer();
}

ImguiHandler* Context::getImguiHandler() const
{
	return Engine::get()->getImguiHandler();
}

Input* Context::getInput() const
{
	return Engine::get()->getInput();
}

EventSystem* Context::getEventSystem() const
{
	return Engine::get()->getEventSystem();
}

ModelImporter* Context::getModelImporter() const
{
	return Engine::get()->getModelImporter();
}

std::shared_ptr<Material> Context::getDefaultMaterial() const
{
	return m_defaultMaterial;
}

void Context::close()
{
	m_scenes.clear();
	m_shaders.clear();
	m_dummyTexture.release();
}