#pragma once
#include <string>

class PlayerAvatar
{
public:
	PlayerAvatar(Ogre::SceneManager* scene_manager, Ogre::SceneNode* camera, std::string mesh_file_name);
	void Update(Ogre::Real delta_time);
	void Move(Ogre::Vector3 translate_vector, Ogre::Real delta_time);
	Ogre::Radian GetRotation(const Ogre::Vector3& vec);
	void SetIdleAnimationLoop();
	void SetWalkAnimationLoop();
	void StopAnimationLoop() const;
	void StartAnimationLoop() const;
	double speed;
private:
	Ogre::SceneManager* _scene_manager;
	Ogre::Entity* _entity;
	Ogre::SceneNode* _entity_node;
	Ogre::AnimationState* _animation_state_base;
	Ogre::AnimationState* _animation_state_top;
};

