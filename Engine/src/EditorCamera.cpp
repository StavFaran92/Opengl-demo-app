#include "EditorCamera.h"

#include <gl/glew.h>

#include "ApplicationConstants.h"
#include "Engine.h"
#include "Context.h"
#include "Scene.h"
#include "EventSystem.h"
#include "Transformation.h"
#include "CameraControllerOrbit.h"
#include "CameraControllerFreeLook.h"

#include "Logger.h"

EditorCamera::EditorCamera()
{
	m_cameraController = std::make_shared<CameraControllerFreeLook>();
}

glm::mat4 EditorCamera::getView()
{
	//return m_cameraComponent->getView();
	return {};
}

glm::vec3 EditorCamera::getPosition()
{
	//return m_cameraComponent->getPosition();
	return{};
}

void EditorCamera::lookAt(float x, float y, float z)
{
}

void EditorCamera::setPosition(float x, float y, float z)
{
	//entity.getComponent<Transformation>().setLocalPosition({ x, y, z } );

	//m_cameraComponent->position = {x,y,z};

	//m_cameraController->calculateOrientation();

	//m_distance = distance;
	//m_angleX = angleX;
	//m_angleY = angleY;

	//calculateOrientation();
}

void EditorCamera::onCreate()
{
	m_cameraController->onCreate(entity);
	m_cameraController->calculateOrientation();
}