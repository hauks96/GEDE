#include "pch.h"
#include "PlayerAvatar.h"


PlayerAvatar::PlayerAvatar(Ogre::SceneManager* scene_manager, Ogre::SceneNode* camera, std::string mesh_file_name)
{
	_scene_manager = scene_manager;
	_entity = _scene_manager->createEntity(mesh_file_name);
	_entity->setCastShadows(true);
	_entity_node = _scene_manager->getRootSceneNode()->createChildSceneNode();
	_entity_node->attachObject(_entity);
	_animation_state_base = nullptr;
	_animation_state_top = nullptr;
	speed = 15;

	camera->setAutoTracking(true, _entity_node, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3(0, 10, -5));
}


void PlayerAvatar::Update(Ogre::Real delta_time)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);

	Ogre::Vector3 translate_vector(0, 0, 0);
	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
		translate_vector += Ogre::Vector3(1, 0, 0);
	}
	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
		translate_vector += Ogre::Vector3(-1, 0, 0);
	}
	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
		translate_vector += Ogre::Vector3(0, 0, 1);
	}
	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
		translate_vector += Ogre::Vector3(0, 0, -1);
	}

	const bool is_walking = translate_vector != Ogre::Vector3(0, 0, 0);

	if (is_walking)
	{
		Move(translate_vector, delta_time);
		SetWalkAnimationLoop();
	}
	else
	{
		SetIdleAnimationLoop();
	}
	_animation_state_base->addTime(delta_time);
	_animation_state_top->addTime(delta_time);
}

void PlayerAvatar::Move(Ogre::Vector3 translate_vector, Ogre::Real delta_time)
{
	_entity_node->translate(speed * delta_time * translate_vector);
	_entity_node->resetOrientation();
	// Rotate the player to face in the direction of the translation
	_entity_node->yaw(GetRotation(translate_vector));
}

Ogre::Radian PlayerAvatar::GetRotation(const Ogre::Vector3& vec)
{
	Ogre::Vector3 v(vec);
	v.normalise();
	return((v.x <= 0 ? -1.0 : 1.0) * Ogre::Radian(acos(v.z)));
}


void PlayerAvatar::SetIdleAnimationLoop()
{
	if (_animation_state_top == nullptr)
	{
		_animation_state_base = _entity->getAnimationState("IdleBase");
		_animation_state_top = _entity->getAnimationState("IdleTop");
		StartAnimationLoop();

	}
	// Stop other animation if there is any already running
	else if (_animation_state_top->getAnimationName() != "IdleTop")
	{
		StopAnimationLoop();
		_animation_state_base = _entity->getAnimationState("IdleBase");
		_animation_state_top = _entity->getAnimationState("IdleTop");
		StartAnimationLoop();
	}
}

void PlayerAvatar::SetWalkAnimationLoop()
{
	if (_animation_state_top == nullptr)
	{
		_animation_state_base = _entity->getAnimationState("RunBase");
		_animation_state_top = _entity->getAnimationState("RunTop");
		StartAnimationLoop();

	}
	// Stop other animation if there is any already running
	else if (_animation_state_top->getAnimationName() != "RunTop")
	{
		StopAnimationLoop();
		_animation_state_base = _entity->getAnimationState("RunBase");
		_animation_state_top = _entity->getAnimationState("RunTop");
		StartAnimationLoop();
	}
}

void PlayerAvatar::StopAnimationLoop() const
{
	if (_animation_state_base != nullptr &&
		_animation_state_base->getEnabled())
	{
		_animation_state_base->setTimePosition(0.0f);
		_animation_state_base->setEnabled(false);
	}
	if (_animation_state_top != nullptr &&
		_animation_state_top->getEnabled())
	{
		_animation_state_top->setTimePosition(0.0f);
		_animation_state_top->setEnabled(false);
	}
}

void PlayerAvatar::StartAnimationLoop() const
{
	if (_animation_state_base != nullptr &&
		!_animation_state_base->getEnabled())
	{
		_animation_state_base->setLoop(true);
		_animation_state_base->setEnabled(true);
	}
	if (_animation_state_top != nullptr &&
		!_animation_state_top->getEnabled())
	{
		_animation_state_top->setLoop(true);
		_animation_state_top->setEnabled(true);
	}
}
