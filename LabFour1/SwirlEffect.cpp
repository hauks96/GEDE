#include "pch.h"
#include "SwirlEffect.h"
#include <math.h>
#include <iostream>

SwirlEffect::SwirlEffect(SceneNode* scene_node, Ogre::Vector3 cylindrical_velocity_vector)
{
	center_pos_ = scene_node->getPosition();
	scene_node_ = scene_node;
	cylindrical_velocity_vector_ = cylindrical_velocity_vector;
	current_cylindrical_pos_ = Ogre::Vector3(0, 0, 0);
	start_cylindrical_pos_ = Ogre::Vector3(0, 0, 0);
	end_cylindrical_pos_ = Ogre::Vector3(5.0, 0, 0);

	effect_running_ = false;
	effect_finished_ = false;
}

Ogre::Vector3 SwirlEffect::getCylindrical() const
{
	return current_cylindrical_pos_;
}

Ogre::Vector3 SwirlEffect::getCartesian() const
{
	return Ogre::Vector3(
		cos(current_cylindrical_pos_.z) * current_cylindrical_pos_.y,
		current_cylindrical_pos_.x,
		sin(current_cylindrical_pos_.z) * current_cylindrical_pos_.y
	);
}

void SwirlEffect::run()
{
	effect_running_ = true;
}

void SwirlEffect::update(float delta_time)
{
	if (effect_running_)
	{

		if (current_cylindrical_pos_.x >= end_cylindrical_pos_.x)
		{
			effect_running_ = false;
			effect_finished_ = true;
		}
		else
		{
			current_cylindrical_pos_ = current_cylindrical_pos_ + delta_time * cylindrical_velocity_vector_;
			scene_node_->setPosition(getCartesian() + center_pos_);
			std::cout << current_cylindrical_pos_ << std::endl;
		}
	}
}

bool SwirlEffect::isRunning()
{
	return effect_running_;
}

bool SwirlEffect::isFinished()
{
	return effect_finished_;
}