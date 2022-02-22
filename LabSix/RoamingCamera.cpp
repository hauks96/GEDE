#include "pch.h"
#include "RoamingCamera.h"
RoamingCamera::RoamingCamera(SceneManager* scene_manager, RenderWindow*
	render_window, Vector3 position, Vector3 lookat_position)
{
	scene_manager_ = scene_manager;
	// Creating and attaching the camera
	camera_yaw_node_ = scene_manager->getRootSceneNode()->createChildSceneNode("camera_yaw_node_");
	camera_pitch_node_ = camera_yaw_node_->createChildSceneNode("camera_pitch_node_");

	camera_ = scene_manager->createCamera("RoamingCamera");
	camera_pitch_node_->attachObject(camera_);

	camera_yaw_node_->setPosition(position);
	camera_pitch_node_->lookAt(lookat_position, Ogre::Node::TransformSpace::TS_WORLD);


	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);

	// Configuring the camera
	camera_->setNearClipDistance(5);
	Ogre::Viewport* vp = render_window->addViewport(camera_);
	vp->setBackgroundColour(fadeColour);
	camera_->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp -> getActualHeight()));

	// Setting motion parameters
	movement_speed_ = 7.0f;
	rotation_speed_ = 0.8f;
	_camera_angle = 0;
}

RoamingCamera::~RoamingCamera()
{
}

void RoamingCamera::update(Ogre::Real delta_time, const Uint8* keyboard_state)
{
	int x = 0, y = 0;
	// Leave if right mouse button is not being pressed
	// ...but also retrieve and store mouse movement
	if (!(SDL_GetRelativeMouseState(&x, &y) & SDL_BUTTON_RMASK)) return;
	// Construct displacement vector
	Ogre::Vector3 vec = Ogre::Vector3(0, 0, 0);
	if (keyboard_state[SDL_SCANCODE_W]) {
		vec = Ogre::Vector3(0, 0, -1);
	}
	if (keyboard_state[SDL_SCANCODE_S]) {
		vec += Ogre::Vector3(0, 0, 1);
	}
	if (keyboard_state[SDL_SCANCODE_A]) {
		vec += Ogre::Vector3(-1, 0, 0);
	}
	if (keyboard_state[SDL_SCANCODE_D]) {
		vec += Ogre::Vector3(1, 0, 0);
	}
	if (keyboard_state[SDL_SCANCODE_Q]) {
		vec += Ogre::Vector3(0, 1, 0);
	}
	if (keyboard_state[SDL_SCANCODE_E]) {
		vec += Ogre::Vector3(0, -1, 0);
	}// Construct view rotation
	float rotX = x * delta_time * -1 * rotation_speed_;
	float rotY = y * delta_time * -1 * rotation_speed_;

	// Update camera with new displacement and rotation
	camera_yaw_node_->yaw(Ogre::Radian(rotX));
	camera_pitch_node_->pitch(Ogre::Radian(rotY));

	Ogre::Vector3 direction = (camera_yaw_node_->getOrientation() * camera_pitch_node_->getOrientation()) * vec;

	camera_yaw_node_->translate(delta_time * movement_speed_ * direction);
}


void RoamingCamera::update(Ogre::Real delta_time, const float camera_movement, const Ogre::Vector3 player_position)
{
	camera_pitch_node_->setFixedYawAxis(true);
	// Update the camera angle based on the joystick axis input
	_camera_angle = delta_time * camera_movement + _camera_angle;

	// Compute the right offset that allows the camera to "orbit" around the player as they rotate
	Vector3 cameraOffset = Ogre::Vector3(30.0f * Ogre::Math::Cos(_camera_angle), 10.0f, 30.0f * Ogre::Math::Sin(_camera_angle));
	camera_yaw_node_->setPosition(player_position + cameraOffset);

	// Rotate the camera "horizontally" to match the player orientation
	float rotX = camera_movement * delta_time * -1;
	camera_yaw_node_->yaw(Ogre::Radian(rotX));

	// Rotate the camera "vertically" to look at the player
	camera_pitch_node_->lookAt(player_position, Ogre::Node::TransformSpace::TS_WORLD);
}

Ogre::Vector2 RoamingCamera::getDirection()
{
	Ogre::Vector2 direction = Ogre::Vector2(Ogre::Math::Cos(_camera_angle), Ogre::Math::Sin(_camera_angle));
	return direction.normalisedCopy();
}