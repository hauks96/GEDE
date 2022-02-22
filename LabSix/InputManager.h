#pragma once
class InputManager
{
public:
	float getCameraMovement() const;
	Ogre::Vector2 getCharacterMovement() const;
	void update();

private:
	SDL_Joystick* _joystick;
	short _player_axis_x;
	short _player_axis_y;
	short _camera_x;
};

