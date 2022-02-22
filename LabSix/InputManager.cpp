#include "pch.h"
#include "InputManager.h"


const int JOYSTICK_DEAD_ZONE = 5000; // TODO: Adjust

InputManager::InputManager()
{
	_joystick = nullptr;
	_player_axis_x = 0;
	_player_axis_y = 0;
	_camera_x = 0;

	// Initialize the joystick subsystem
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	// Check for joystick
	if (SDL_NumJoysticks() > 0) {
		// Open joystick
		_joystick = SDL_JoystickOpen(0);

		if (_joystick) {
			printf("Opened Joystick 0\n");
			printf("Name: %s\n", SDL_JoystickNameForIndex(0));
			printf("Number of Axes: %d\n", SDL_JoystickNumAxes(_joystick));
			printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(_joystick));
			printf("Number of Balls: %d\n", SDL_JoystickNumBalls(_joystick));
		}
		else {
			printf("Couldn't open Joystick 0\n");
		}
	}
}

InputManager::~InputManager()
{
	// Close if opened
	if (SDL_JoystickGetAttached(_joystick)) {
		SDL_JoystickClose(_joystick);
	}
}

float InputManager::getCameraMovement() const
{
	return Ogre::Math::Abs(_camera_x) > JOYSTICK_DEAD_ZONE ? _camera_x / 32767.0 : 0;
}

Ogre::Vector2 InputManager::getCharacterMovement() const
{
	Ogre::Real x = Ogre::Math::Abs(_player_axis_x) > JOYSTICK_DEAD_ZONE ? _player_axis_x / 32767.0 : 0.0;
	Ogre::Real y = Ogre::Math::Abs(_player_axis_y) > JOYSTICK_DEAD_ZONE ? _player_axis_y / 32767.0 : 0.0;

	return Ogre::Vector2(x, y);
}

void InputManager::update()
{
	_player_axis_x = SDL_JoystickGetAxis(_joystick, 0);
	_player_axis_y = SDL_JoystickGetAxis(_joystick, 1);
	_camera_x = SDL_JoystickGetAxis(_joystick, 5);


	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_JOYAXISMOTION)
		{
			std::cout << "WHICH: " << event.jaxis.which << std::endl;
			std::cout << "AXIS: " << event.jaxis.axis << std::endl;

			if (event.jaxis.which == 0)
			{
				if (event.jaxis.axis == 0)
				{
					const auto value = event.jaxis.value;
					_player_axis_x = value > _player_axis_x ? value : _player_axis_x;
				}
				if (event.jaxis.axis == 1)
				{
					const auto value = event.jaxis.value;
					_player_axis_y = value > _player_axis_y ? value : _player_axis_y;
				}
			}
			if (event.jaxis.which == 5)
			{
			}
		}
	}
}