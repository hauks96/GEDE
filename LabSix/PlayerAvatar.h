#pragma once
#include <string>

class PlayerAvatar
{
public:
	PlayerAvatar(Ogre::SceneManager* scene_manager, std::string mesh_file_name);
	void Update(Ogre::Real delta_time, const Uint8* state);
	void Update(Ogre::Real delta_time, const Ogre::Vector2 camera_direction, const Ogre::Vector2 character_movement);
	void Move(Ogre::Vector3 translate_vector, float rotation, Ogre::Real delta_time);
	Ogre::Radian GetRotation(const Ogre::Vector3& vec);
	void SetIdleAnimationLoop();
	void SetWalkAnimationLoop();
	void StopAnimationLoop() const;
	void StartAnimationLoop() const;
	Ogre::SceneNode* getEntityNode();
private:
	Ogre::SceneManager* _scene_manager;
	Ogre::Entity* _entity;
	Ogre::SceneNode* _entity_node;
	Ogre::AnimationState* _animation_state_base;
	Ogre::AnimationState* _animation_state_top;
	float _speed;
	float _rotation;
	float _rotation_speed;
};

