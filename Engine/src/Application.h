#pragma once
#include <memory>

#include "Core.h"
#include "SDL2/SDL_scancode.h"

class Model;
class Shader;
class Skybox;
class Box;
class Quad;
class Sphere;
class Object3D;
class Context;
template<typename> class ObjectHandler;

class EngineAPI Application
{
public:
	virtual void start() = 0;
	virtual void update() {};
	virtual void close() {};

protected:
	/** API */
	Box* createBoxEntity();
	Quad* createQuad();
	void postProcess(Shader*);
	void skybox(std::shared_ptr<Skybox> skybox);

	Context* getContext() const;

	// Input
	int keyboard_getKeyState(SDL_Scancode code);
	bool mouse_leftButtonPressed();
	bool mouse_rightButtonPressed();
	bool mouse_middleButtonPressed();

	Application() = default;
	~Application() = default;

private:


};

Application* CreateApplication();



