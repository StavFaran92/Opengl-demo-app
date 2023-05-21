#include "EntryPoint.h"
#include "sge.h"

#include "GUIHandler.h"

class Sandbox : public Application
{
public:
	using Application::draw;
	using Application::update;


	ObjectHandler<Model> plane;
	ObjectHandler<Model> light;
	std::shared_ptr<GUIHandler> gui;


	void start() override
	{
		auto importer = getContext()->getModelImporter();
		plane = importer->loadModelFromFile("C:/Users/Stav/Documents/blender/plane_v2.fbx");

		plane.object()->rotate({1,0,0}, 90);
		plane.object()->rotate({0,1,0}, 90);

		StandardShader* shader = Shader::create<StandardShader>("Resources/Shaders/OceanVertexShader.vert", "Resources/Shaders/OceanFragmentShader.frag");
		shader->setValue("amplitude", .5f);
		shader->setValue("waveDirection", glm::vec2(1,0));
		shader->setValue("waveLength", 2.f);
		shader->setValue("waveSpeed", 5.0f);
		plane.object()->attachShader(shader);

		auto dLight = new PointLight(glm::vec3{ 1,1,1 }, glm::vec3{ 0,1,-1 }, 1, 1, Attenuation());
		getContext()->getActiveScene()->addPointLight(dLight);

		gui = std::make_shared<GUIHandler>(shader);

	}

	void update(float deltaTime) override
	{
		update(plane);
		//update(guitar);
		//update(sphere1);
		//update(sphere2);
	}

	void draw() override
	{
		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_X))
		//{
		//	box->rotate({0,0,1}, 1);
		//}

		//if (keyboard_getKeyState(SDL_Scancode::SDL_SCANCODE_Z))
		//{
		//	sphere1->rotate({ 0,0,1 }, 1);
		//}

		getContext()->getActiveScene()->draw(plane);

		Engine::get()->getImguiHandler()->draw(gui.get());

		//draw(box);
		//draw(guitar);
		//draw(sphere1);
		//draw(sphere2);
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}