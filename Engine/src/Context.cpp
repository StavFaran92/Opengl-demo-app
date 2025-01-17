#include "Context.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "Skybox.h"
#include "SkyboxRenderer.h"
#include "ICamera.h"
#include "PhongShader.h"
#include "PickingShader.h"
#include "Shader.h"

#include "Scene.h"
#include "Engine.h"

#include "Logger.h"
#include "Engine.h"
#include "Texture.h"
#include "Material.h"
#include "ProjectAssetRegistry.h"
#include "Graphics.h"

Context::Context(const std::shared_ptr<ProjectAssetRegistry>& par)
{
	m_projectAssetRegistry = par;

	m_orphanRegistry = std::make_shared<SGE_Regsitry>();
}

void Context::init()
{
	
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
	//auto defaultScene = std::make_shared<Scene>(this);

	//addScene(defaultScene);
	//m_activeScene = defaultScene->getID();

	//defaultScene->deserialize();
}

SGE_Regsitry& Context::getRegistry() const
{
	return *m_orphanRegistry.get();
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

//Resource<Texture> Context::getDummyTexture()
//{
//	return m_dummyTexture;
//}

const std::map<uint32_t, std::shared_ptr<Scene>>& Context::getAllScenes() const
{
	return m_scenes;
}

uint32_t Context::getActiveSceneID() const
{
	return m_activeScene;
}

void Context::save() const
{
	m_projectAssetRegistry->save();

	logInfo("Successfully serialized Context.");
}

void Context::setRenderMode(RenderMode mode)
{
	Engine::get()->getSubSystem<Graphics>()->renderMode = mode;
}

RenderMode Context::getRenderMode() const
{
	return Engine::get()->getSubSystem<Graphics>()->renderMode;
}

void Context::update(float deltaTime)
{
	if (m_activeScene == -1)
		return;

	//if (!m_scenes[m_activeScene]->isSimulationActive())
	//{
	//	m_scenes[m_activeScene]->startSimulation();
	//}

	m_scenes[m_activeScene]->update(deltaTime);
}

void Context::draw(float deltaTime)
{
	if (m_activeScene == -1)
		return;

	m_scenes[m_activeScene]->draw(deltaTime);
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

//std::shared_ptr<Material> Context::getDefaultMaterial() const
//{
//	return m_defaultMaterial;
//}

void Context::close()
{
	m_scenes.clear();
	m_shaders.clear();
}