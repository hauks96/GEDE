#include "pch.h"
#include "PlayerAvatar.h"


PlayerAvatar::PlayerAvatar(Ogre::SceneManager* scene_manager, std::string mesh_file_name)
{
	_scene_manager = scene_manager;
	_entity = _scene_manager->createEntity(mesh_file_name);
	_entity->setCastShadows(true);
	_entity_node = _scene_manager->getRootSceneNode()->createChildSceneNode();
	_entity_node->attachObject(_entity);
	_animation_state_base = nullptr;
	_animation_state_top = nullptr;
	_speed = 6.0;
	_rotation = 0.0;
	_rotation_speed = 5.0f;
}


void PlayerAvatar::Update(Ogre::Real delta_time, const Uint8* state)
{
	// Leave if the right mouse button is being pressed (different function)
	if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK)) return;
	// Construct the movement
	Ogre::Vector3 translate_vector(0, 0, 0);
	Ogre::Vector3 dir(sin(_rotation), 0, cos(_rotation));

	if (state[SDL_SCANCODE_W]) {
		translate_vector = _speed * dir;
	}
	if (state[SDL_SCANCODE_S]) {
		translate_vector = _speed * -1 * dir;
	}
	if (state[SDL_SCANCODE_A]) {
		_rotation += delta_time * _rotation_speed;
	}
	if (state[SDL_SCANCODE_D]) {
		_rotation -= delta_time * _rotation_speed;
	}

	Move(translate_vector, _rotation, delta_time);

	if (!translate_vector.isZeroLength())
	{
		SetWalkAnimationLoop();
	}
	else
	{
		SetIdleAnimationLoop();
	}
	_animation_state_base->addTime(delta_time);
	_animation_state_top->addTime(delta_time);
}

void PlayerAvatar::Update(Ogre::Real delta_time, const Ogre::Vector2 camera_direction, const Ogre::Vector2 character_movement)
{
	Ogre::Vector3 translate(camera_direction.x * character_movement.y, 0, camera_direction.y * character_movement.y);
	translate = Ogre::Vector3(translate.x + camera_direction.y * character_movement.x, 0, translate.z - camera_direction.x * character_movement.x);
	translate = translate.normalisedCopy();
	translate = translate * _speed;

	// Keep player facing away from the camera
	_rotation = (GetRotation(Ogre::Vector3(camera_direction.x, 0, camera_direction.y)) + Ogre::Radian(Ogre::Math::PI)).valueRadians();

	Move(translate, _rotation, delta_time);

	if (!translate.isZeroLength())
	{
		SetWalkAnimationLoop();
	}
	else
	{
		SetIdleAnimationLoop();
	}
	_animation_state_base->addTime(delta_time);
	_animation_state_top->addTime(delta_time);
}

void PlayerAvatar::Move(Ogre::Vector3 translate_vector, float rotation, Ogre::Real delta_time)
{
	_entity_node->translate(_speed * delta_time * translate_vector);
	_entity_node->resetOrientation();
	// Rotate the player to face in the direction of the translation
	_entity_node->yaw(Ogre::Radian(rotation));
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

Ogre::SceneNode* PlayerAvatar::getEntityNode()
{
	return _entity_node;
}
