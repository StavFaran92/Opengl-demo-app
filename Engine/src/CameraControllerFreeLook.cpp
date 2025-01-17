#include "CameraControllerFreeLook.h"

#include "Engine.h"
#include "EventSystem.h"
#include "ApplicationConstants.h"
#include "Transformation.h"
#include "System.h"
#include <algorithm>
#include "glm/glm.hpp"

void CameraControllerFreeLook::calculateOrientation()
{
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(m_front);
	m_cameraComponent->center = m_cameraTransform->getLocalPosition() + m_front;

	m_right = glm::normalize(glm::cross(m_front, m_up));
	m_cameraComponent->front = m_front;
	m_cameraComponent->right = m_right;
	m_cameraComponent->up = glm::normalize(glm::cross(m_right, m_front));
}

void CameraControllerFreeLook::onCreate(Entity& e)
{
	m_cameraComponent = &e.getComponent<CameraComponent>();
	m_cameraTransform = &e.getComponent<Transformation>();

	auto eventSystem = Engine::get()->getEventSystem();

	eventSystem->addEventListener(SDL_MOUSEMOTION, [this](SDL_Event e)
		{
			auto system = Engine::get()->getSubSystem<System>();

			if (m_state == ControllerState::ROTATE)
			{
				float xChange = e.motion.xrel;
				float yChange = e.motion.yrel;

				xChange *= m_turnSpeed * system->getDeltaTime();
				yChange *= m_turnSpeed * system->getDeltaTime();

				m_yaw += xChange;
				m_pitch -= yChange;

				if (m_pitch > 89.0f)
				{
					m_pitch = 89.0f;
				}

				if (m_pitch < -89.0f)
				{
					m_pitch = -89.0f;
				}

				calculateOrientation();
			}

			if (m_state == ControllerState::TRANSFORM)
			{
				auto system = Engine::get()->getSubSystem<System>();

				float xChange = e.motion.xrel;
				float yChange = e.motion.yrel;

				xChange *= m_movementSpeed * system->getDeltaTime();
				yChange *= m_movementSpeed * system->getDeltaTime();

				float xVelocity = .1f * xChange;
				float yVelocity = .1f * yChange;


				m_cameraTransform->translate(m_right * xVelocity);
				m_cameraTransform->translate(-m_up * yVelocity);

				calculateOrientation();
			}
		});
	eventSystem->addEventListener(SDL_MOUSEBUTTONDOWN, [this](SDL_Event e){
		if (e.button.button == SDL_BUTTON_RIGHT)
		{
			if (m_state == ControllerState::IDLE)
			{
				m_state = ControllerState::ROTATE;
			}
		}

		else if (e.button.button == SDL_BUTTON_MIDDLE)
		{
			if (m_state == ControllerState::IDLE)
			{
				m_state = ControllerState::TRANSFORM;
			}
		}
	});
	eventSystem->addEventListener(SDL_MOUSEBUTTONUP, [this](SDL_Event e){
		if (e.button.button == SDL_BUTTON_RIGHT || e.button.button == SDL_BUTTON_MIDDLE)
		{
			m_state = ControllerState::IDLE;
		}
	});
	eventSystem->addEventListener(SDL_MOUSEWHEEL, [this](SDL_Event e){
		m_cameraTransform->translate(m_front * (float)e.wheel.y);

		calculateOrientation();
	});
}
