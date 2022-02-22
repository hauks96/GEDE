#pragma once
#include "pch.h"
#include "PlayerAvatar.h"
#include "RoamingCamera.h"
#include "InputManager.h"

using namespace Ogre;
using namespace OgreBites;

class MyEngine : public ApplicationContext, public InputListener
{
public:
	MyEngine();
	virtual ~MyEngine() {};
	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	void setupSceneManager();
	void setupCamera();
	void setupInputManager();
	void populateScene();
	bool frameStarted(const Ogre::FrameEvent& evt);
private:
	Ogre::Root* _root;
	Ogre::SceneManager* _scene_manager;
	RoamingCamera* _roaming_camera;
	Ogre::SceneNode* _camera_node;
	PlayerAvatar* _player;
	InputManager* _input_manager;
};
