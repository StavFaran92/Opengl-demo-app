#include "Engine.h"

#include "Context.h"
#include "Renderer.h"
#include "SkyboxRenderer.h"
#include "Window.h"
#include "ImguiHandler.h"
#include "ObjectSelection.h"
#include "PostProcessProjector.h"
#include "ICamera.h"
#include "Scene.h"
#include "Skybox.h"
#include "Input.h"
#include "EventSystem.h"
#include "ModelImporter.h"
#include "Logger.h"
#include "MemoryManagement.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"
#include "TimeManager.h"
#include "PhysicsSystem.h"
#include "Random.h"
#include "ShadowSystem.h"
#include "ShaderLoader.h"
#include "ShaderParser_tntmeijsImpl.h"
#include "ResourceManager.h"

#include "Application.h"
#include "SDL2/SDL.h"

#include "GL/glew.h"

// Singleton
Engine* Engine::instance = nullptr;

bool Engine::init()
{
    if (m_isInit)
    {
        logError("Engine already started!");
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    m_eventSystem = std::make_shared<EventSystem>();

    m_memoryManagementSystem = std::make_shared<MemoryManagement>();

    m_resourceManager = std::make_shared<ResourceManager>();

    if (!SGE_EXPORT_PACKAGE)
    {
        auto found = false;
        try
        {
            size_t len = 0;
            char* sgeRoot = nullptr;
            errno_t err = _dupenv_s(&sgeRoot, &len, "SGE");
            if (err == 0 && sgeRoot)
            {
                found = true;
                m_resourceManager->setRootDir(std::string(sgeRoot) + "/Engine/");
            }
            free(sgeRoot);
        }
        catch (std::exception e)
        {
            logError(e.what());
        }

        if (!found)
        {
            m_resourceManager->setRootDir("./");
        }
        
    }
    else
    {
        m_resourceManager->setRootDir("./");
    }

    m_objectManager = std::make_shared<ObjectManager>();

    ObjectFactory::init(m_objectManager.get());

    m_window = std::make_shared<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!m_window->init())
    {
        logError("Window init failed!");
        return false;
    }

    m_input = std::make_shared<Input>();
    m_input->init();

    auto shaderParser = std::make_shared<ShaderParser_tntmeijs>();

    ShaderLoader::LoadParams lParams;
    lParams.extendShader = true;
    m_shaderLoader = std::make_shared<ShaderLoader>(shaderParser, lParams);

    m_context = std::make_shared<Context>();

    //auto secondScene = std::make_shared<Scene>();
    //secondScene->setPostProcess(true);
    //m_context->addScene(secondScene);

    m_imguiHandler = std::make_shared<ImguiHandler>();
    if (!m_imguiHandler->init(m_window->GetWindow(), m_window->GetContext()))
    {
        logError("Imgui init failed!");
        return false;
    }

    m_modelImporter = std::make_shared<ModelImporter>();

    m_timeManager = std::make_shared<TimeManager>();
    m_physicsSystem = std::make_shared<PhysicsSystem>();
    if (!m_physicsSystem->init())
    {
        logError("Physics System init failed!");
        return false;
    }

    m_randomSystem = std::make_shared<RandomNumberGenerator>();

    auto defaultScene = std::make_shared<Scene>(m_context.get());
    //defaultScene->setPostProcess(true);

    m_context->addScene(defaultScene);
    m_context->setActiveScene(defaultScene->getID());

    m_isInit = true;

    logInfo("SGE Initialized Successfully!");

    return true;
}

Window* Engine::getWindow() const
{
    return m_window.get();
}

Engine::Engine()
{}

Engine* Engine::get()
{
    if (instance == nullptr)
    {
        instance = new Engine();
    }
    return instance;
}

std::string Engine::getRootDir()
{
    return m_resourceManager->getRootDir();
}

void Engine::SetWindow(std::shared_ptr<Window> window)
{
    m_window = window;
}

IRenderer* Engine::getRenderer() const
{
    return m_context->getActiveScene()->getRenderer().get();
}

void Engine::SetContext(std::shared_ptr<Context> context)
{
    m_context = context;
}

Context* Engine::getContext() const
{
    return m_context.get();
}


void Engine::draw(float deltaTime)
{
    m_context->draw(deltaTime);

    m_imguiHandler->render();

    m_window->SwapBuffer();
}


void Engine::update(float deltaTime)
{
    m_context->update(deltaTime);
}

void Engine::run(Application* app)
{
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    // Loop until window closed
    while (!quit)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)SDL_GetPerformanceFrequency() / ((NOW - LAST) * 1000);

        //Handle events on queue
        handleEvents(e, quit);

        if (quit)
            return;

        update(deltaTime);
        draw(deltaTime);

    }
}

void Engine::stop()
{
    logTrace(__FUNCTION__);

    SDL_Event e;
    e.type = SDL_QUIT;
    m_eventSystem->pushEvent(e);
}

void Engine::close()
{
    logTrace(__FUNCTION__);

    m_imguiHandler->close();

    m_window->close();

    m_isInit = false;

    delete instance;
    instance = nullptr;
}

ImguiHandler* Engine::getImguiHandler() const
{
    return m_imguiHandler.get();
}

Input* Engine::getInput() const
{
    return m_input.get();
}

EventSystem* Engine::getEventSystem() const
{
    return m_eventSystem.get();
}

ModelImporter* Engine::getModelImporter() const
{
    return m_modelImporter.get();
}

MemoryManagement* Engine::getMemoryManagementSystem() const
{
    return m_memoryManagementSystem.get();
}

ObjectManager* Engine::getObjectManager() const
{
    return m_objectManager.get();
}

TimeManager* Engine::getTimeManager() const
{
    return m_timeManager.get();
}

PhysicsSystem* Engine::getPhysicsSystem() const
{
    return m_physicsSystem.get();
}

RandomNumberGenerator* Engine::getRandomSystem() const
{
    return m_randomSystem.get();
}

ShaderLoader* Engine::getShaderLoader() const
{
    return m_shaderLoader.get();
}

ResourceManager* Engine::getResourceManager() const
{
    return m_resourceManager.get();
}

void Engine::pause()
{
}

void Engine::handleEvents(SDL_Event& e, bool& quit)
{
    while (SDL_PollEvent(&e) != 0)
    {
        m_imguiHandler->proccessEvents(e);

        //User requests quit
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }


        m_eventSystem->dispatch(e);
    }
}

void Engine::resume()
{
}
